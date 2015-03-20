asm(".code16gcc");
#include "fat.h"
#include "types.h"
#include "malloc.h"
#include "bios.h"
#include "common.h"
#include "string.h"

int xpread( void *buf, int size,int sector)
{
	int count = size / LIBFAT_SECTOR_SIZE;
	sector += 0x3f;
	pread( buf, count, sector);
	return size;
}
filesystem* initFileSystem(){
	filesystem * fs = NULL;
	struct fat_bootsect * bs;

	if(fs == NULL){
		fs = (filesystem *)malloc(sizeof(filesystem));
	}
	fs->sectors = NULL;
	fs->read = xpread;
	
	bs = getSector( fs,0);
	if (!bs)
		goto barf;
	if (read16(&bs->bsBytesPerSec) != LIBFAT_SECTOR_SIZE)
		goto barf;
	int i = 0;
	for (; i <= 8; i++) {
		if ((uint8_t) (1 << i) == read8(&bs->bsSecPerClust))
			break;
	}
	if (i > 8)
		goto barf;
	fs->clustsize = 1 << i;	/* Treat 0 as 2^8 = 64K */
	fs->clustshift = i;

    int sectors = read16(&bs->bsSectors);
    if (!sectors)
		sectors = read32(&bs->bsHugeSectors);

    fs->end = sectors;

    fs->fat = read16(&bs->bsResSectors);
    int fatsize = read16(&bs->bsFATsecs);
	if (!fatsize)
		fatsize = read32(&bs->u.fat32.bpb_fatsz32);

    fs->rootdir = fs->fat + fatsize * read8(&bs->bsFATs);

	int rootdirsize = ((read16(&bs->bsRootDirEnts) << 5) + LIBFAT_SECTOR_MASK) >> LIBFAT_SECTOR_SHIFT;
    fs->data = fs->rootdir + rootdirsize;

    /* Sanity checking */
	if (fs->data >= fs->end)
		goto barf;

    /* Figure out how many clusters */
    int nclusters = (fs->end - fs->data) >> fs->clustshift;
    fs->endcluster = nclusters + 2;
	int minfatsize;
	if (nclusters <= 0xff4) {
		fs->fat_type = FAT12;
		minfatsize = fs->endcluster + (fs->endcluster >> 1);
	} else if (nclusters <= 0xfff4) {
		fs->fat_type = FAT16;
		minfatsize = fs->endcluster << 1;
	} else if (nclusters <= 0xffffff4) {
		fs->fat_type = FAT28;
		minfatsize = fs->endcluster << 2;
	} else
		goto barf;		/* Impossibly many clusters */

    minfatsize = (minfatsize + LIBFAT_SECTOR_SIZE - 1) >> LIBFAT_SECTOR_SHIFT;

	if (minfatsize > fatsize)
		goto barf;		/* The FATs don't fit */

	if (fs->fat_type == FAT28)
		fs->rootcluster = read32(&bs->u.fat32.bpb_rootclus);
	else
		fs->rootcluster = 0;

	fs->curdir = fs->rootdir;
	return fs;
barf:
	return NULL;
}



void * getSector(filesystem *fs, int n){
	sector *ls;

	for (ls = fs->sectors; ls; ls = ls->next) {
		if (ls->n == n)
			return ls->data;    /* Found in cache */
	}
	ls = malloc(sizeof(sector));
	if (!ls) {
		fatFlush(fs);
		ls = malloc(sizeof(sector));

		if (!ls)
			return NULL;    /* Can't allocate memory */
	}
	if (fs->read( ls->data, LIBFAT_SECTOR_SIZE, n)
			!= LIBFAT_SECTOR_SIZE) {
		return NULL;            /* I/O error */
	}
	ls->n = n;
	ls->next = fs->sectors;
	fs->sectors = ls;
	
	return ls->data;
}

void fatFlush(filesystem * fs)
{
	sector *ls, *lsnext;

	lsnext = fs->sectors;
	fs->sectors = NULL;

	for (ls = lsnext; ls; ls = lsnext) {
		lsnext = ls->next;
		free(ls);
	}
}

int clustertosector(filesystem *fs,int32_t cluster)
{       
	if (cluster == 0)
		cluster = fs->rootcluster;

	if (cluster == 0)
		return fs->rootdir;
	else if (cluster < 2 || cluster >= fs->endcluster)
		return -1;  
	else            
		return fs->data + ( (cluster - 2) << fs->clustshift);
}

int  nextsector(filesystem * fs,int s)
{
    int32_t cluster, nextcluster;
    uint32_t fatoffset;
    int fatsect;
    uint8_t *fsdata;
    uint32_t clustmask = fs->clustsize - 1;
	int rs;

	if (s < fs->data) {
		if (s < fs->rootdir)
			return -1;

		/* Root directory */
		s++;
		return (s < fs->data) ? s : 0;
	}

	rs = s - fs->data;

	if (~rs & clustmask)
		return s + 1;		/* Next sector in cluster */

	cluster = 2 + (rs >> fs->clustshift);

	if (cluster >= fs->endcluster)
		return -1;

	switch (fs->fat_type) {
		case FAT12:
			/* Get first byte */
			fatoffset = cluster + (cluster >> 1);
			fatsect = fs->fat + (fatoffset >> LIBFAT_SECTOR_SHIFT);
			fsdata = getSector( fs,fatsect);
			if (!fsdata)
				return -1;
			nextcluster = fsdata[fatoffset & LIBFAT_SECTOR_MASK];

			/* Get second byte */
			fatoffset++;
			fatsect = fs->fat + (fatoffset >> LIBFAT_SECTOR_SHIFT);
			fsdata = getSector( fs,fatsect);
			if (!fsdata)
				return -1;
			nextcluster |= fsdata[fatoffset & LIBFAT_SECTOR_MASK] << 8;

			/* Extract the FAT entry */
			if (cluster & 1)
				nextcluster >>= 4;
			else
				nextcluster &= 0x0FFF;

			if (nextcluster >= 0x0FF8)
				return 0;
			break;

		case FAT16:
			fatoffset = cluster << 1;
			fatsect = fs->fat + (fatoffset >> LIBFAT_SECTOR_SHIFT);
			fsdata = getSector(fs, fatsect);
			if (!fsdata)
				return -1;
			nextcluster =
				read16((le16_t *) & fsdata[fatoffset & LIBFAT_SECTOR_MASK]);

			if (nextcluster >= 0x0FFF8)
				return 0;
			break;

		case FAT28:
			fatoffset = cluster << 2;
			fatsect = fs->fat + (fatoffset >> LIBFAT_SECTOR_SHIFT);
			fsdata = getSector(fs,fatsect);
			if (!fsdata)
				return -1;
			nextcluster =
				read32((le32_t *) & fsdata[fatoffset & LIBFAT_SECTOR_MASK]);
			nextcluster &= 0x0FFFFFFF;

			if (nextcluster >= 0x0FFFFFF8)
				return 0;
			break;

		default:
			return -1;		/* WTF? */
	}

	return clustertosector(fs, nextcluster);
}

int getFileName(struct fat_slot * fatslot,char * fileName){
	int i = 0,j = 0;
	char tmp = NULL;
	if(fatslot->name0 != 0){
		for( i = 0,j=0;i < 5 ;i++,j++){
			tmp = fatslot->name0[j][0];
			fileName[i] = tmp;
		}
		for( i = 5,j=0;i < 11 ;i++,j++){
			tmp = fatslot->name5[j][0];
			fileName[i] = tmp;
			if(tmp == NULL){
				return 1;
			}
		}
		for( i = 11,j=0;i < 13 ;i++,j++){
			tmp = fatslot->name11[j][0];
			fileName[i] = tmp;
			if(tmp == NULL){
				return 1;
			}
		}
	}
	return 1;
}

struct fat_dirent * getDirent(filesystem * fs,int n){
	struct fat_dirent * dirent;
	dirent = getSector(fs, n);
	if (!dirent)
		return NULL;        /* Read error */
	return dirent;
}

int getClusNum(struct fat_dirent * dirent){
	int clusNum = (dirent->clusthi[1] << 8) | dirent->clusthi[0];
	clusNum *= 256 * 256;
	clusNum += (dirent->clustlo[1] << 8) | dirent->clustlo[0];
	return clusNum;
}
int getFileSize(struct fat_dirent * dirent){
	int size = (dirent->size[3] << 24) | (dirent->size[2] << 16) | (dirent->size[1] << 8) | dirent->size[0]  ;
	return size;
}

int listFileStat(filesystem * fs,filestat *stat,int curSize){
	struct fat_dirent * dirent = NULL;
	struct fat_slot *slot = NULL,*tslot = NULL;
	int s = curSize;
	char fileName[256];
	int statSize = 0;
	int nent = 0,slotId = 0;
	while(1){
		if (s == 0){
			break;      /* Not found */
		}
		slot = getSector(fs, s);
		if (!slot)
			return -1;      /* Read error */

		int slotMaxNum = 0;
		int slotNum = 0;
		for (nent = 0; nent < LIBFAT_SECTOR_SIZE;
				nent += sizeof(struct fat_slot) * 2) {
			if(slot->name0[0][0] == 0){ 
				break;
			}
			memset(fileName,0,256);
			slotId = slot->id;
			if(slotId == 0x2e){
				dirent = (struct fat_dirent *)slot ;
				stat->start_index = getClusNum(dirent);
				stat->size = getFileSize(dirent);
				stat->isDir = dirent->attribute / 16;
				strcpy(stat->filename,".");
				slot++;
				stat++;
				dirent = (struct fat_dirent *)slot ;
				stat->start_index = getClusNum(dirent);
				stat->size = getFileSize(dirent);
				stat->isDir = dirent->attribute / 16;
				strcpy(stat->filename,"..");
				statSize += 2;
			}else{
				slotMaxNum = slotId%16;
				for(slotNum = 1;slotNum < slotMaxNum ; slotNum++){
					slot++;
				}
				tslot = slot;
				int fileNameLen = 0;
				for(;slotNum > 0;slotNum--,slot--){
					getFileName(slot,fileName+fileNameLen);
					fileNameLen = strlen(fileName);
					trim(fileName,fileNameLen);
				}
				slot = tslot;
				slot++;
				dirent = (struct fat_dirent *)slot ;
				stat->start_index = getClusNum(dirent);
				stat->size = getFileSize(dirent);
				stat->isDir = dirent->attribute / 16;
				strcpy(stat->filename,fileName);
				statSize++;
			}
			slot++;
			stat++;
		}

		s = nextsector(fs, s);
	}
	return statSize;
}
