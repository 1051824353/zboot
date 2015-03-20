#ifndef LIBFAT_H
#define LIBFAT_H

#include "types.h"

#define LIBFAT_SECTOR_SIZE  512
#define LIBFAT_SECTOR_MASK  511
#define LIBFAT_SECTOR_SHIFT 9

typedef struct libfat_sector sector;
struct libfat_sector {
	int n;          /* Sector number */
	struct libfat_sector *next; /* Next in list */
	char data[LIBFAT_SECTOR_SIZE];
};   

enum fat_type {
	FAT12,
	FAT16,
	FAT28
};

typedef struct fat_filesystem filesystem;

struct fat_filesystem {
	int (*read) ( void *, int, int);
	int readptr;

	enum fat_type fat_type;
	unsigned int clustsize;
	int clustshift;
	int endcluster;         /* Highest legal cluster number + 1 */
	int rootcluster;        /* Root directory cluster */

	int fat;        /* Start of FAT */
	int rootdir;    /* Start of root directory */
	int curdir;
	int data;       /* Start of data area */
	int end;        /* End of filesystem */

	struct libfat_sector *sectors;
};



/* The poor excuse FAT has for a superblock -- in the boot sector */
struct fat_bootsect {
    le8_t bsJump[3];		/* Jump to code */
    char bsOemName[8];		/* Formatting program */
    le16_t bsBytesPerSec;	/* Bytes/sector */
    le8_t bsSecPerClust;	/* Sectors/cluster */
    le16_t bsResSectors;	/* Reserved sectors */
    le8_t bsFATs;		/* Number of FATs */
    le16_t bsRootDirEnts;	/* Number of entries/root directory */
    le16_t bsSectors;		/* Number of sectors [1] */
    le8_t bsMedia;		/* Magic media type byte */
    le16_t bsFATsecs;		/* Sectors/FAT */
    le16_t bsSecPerTrack;	/* Sectors/track */
    le16_t bsHeads;		/* Number of heads */
    le32_t bsHiddenSecs;	/* Number of hidden sectors */
    le32_t bsHugeSectors;	/* Number of sectors [2] */
    union {
	/* FAT12/16 */
	struct {
	    le8_t bsDriveNumber;	/* Drive number */
	    le8_t bsReserved1;	/* Reserved */
	    le8_t bsBootSignature;	/* 0x29 */
	    le32_t bsVolumeID;	/* Volume serial number */
	    char bsVolumeLabel[11];	/* Volume name */
	    char bsFileSysType[8];	/* File system type */

	    le8_t bsCode[448];	/* Boot sector code */
	} fat16;

	/* FAT32 */
	struct {
	    le32_t bpb_fatsz32;	/* Sectors/FAT */
	    le16_t bpb_extflags;	/* Extended flags */
	    le16_t bpb_fsver;	/* Filesystem version */
	    le32_t bpb_rootclus;	/* Root directory cluster */
	    le16_t bpb_fsinfo;	/* FSINFO sector number */
	    le16_t bpb_bkbootsec;	/* Backup boot sector (superblock) */
	    char bpb_reserved[12];

	    /* Same shit, different offset! */
	    le8_t bsDriveNumber;	/* Drive number */
	    le8_t bsReserved1;	/* Reserved */
	    le8_t bsBootSignature;	/* 0x29 */
	    le32_t bsVolumeID;	/* Volume serial number */
	    char bsVolumeLabel[11];	/* Volume name */
	    char bsFileSysType[8];	/* File system type */

	    le8_t bsCode[420];	/* Boot sector code */
	} fat32;
    } u;

    le16_t bsSignature;		/* 0xAA55 */
};

#define BS_BOOTSIGNATURE	0x29
#define BS_SIGNATURE		0xAA55

/* A FAT filesystem directory entry */
struct fat_slot{
	le8_t id;			/* Sequence number for slot */
	le16_t name0[5];		/* 5 characters */
	le8_t attribute;		/* Attribute byte */
	le8_t reserved;		/* Reserved, MBZ */
	le8_t alias_csum;		/* Short name checksum */
	le16_t name5[6];		/* 6 characters */
	le16_t firstclust;		/* MBZ */
	le16_t name11[2];		/* 2 characters */
};
struct fat_dirent{
	le8_t name[8];		/* Mangled filename */
	le8_t suffix[3];
	le8_t attribute;		/* File type/attribute */
	le8_t caseflags;		/* VFAT: case for basis and extension */
	le8_t ctime_ms;		/* ms of creation time */
	le32_t ctime;		/* Creation time */
	le16_t atime;		/* Date portion (high 16 bits) of atime */
	le16_t clusthi;		/* FAT32: high 16 bits of cluster */
	le32_t mtime;		/* Modification time */
	le16_t clustlo;		/* First cluster pointer */
	le32_t size;		/* File size (bytes) */
} ;


typedef struct file_stat filestat;

struct file_stat{
	char filename[256];
	int start_index;
	int size;
	int isDir;
};

filesystem * initFileSystem();
void fatFlush(filesystem * fs);
void*getSector(filesystem *fs,int n);
int  nextsector(filesystem *fs,int s);
int clustertosector(filesystem *fs,int32_t cluster);
int xpread( void *buf, int size,int sector);
int listFileStat(filesystem * fs,filestat *stat,int curSize);
struct fat_dirent * getDirent(filesystem * fs,int n);
int getClusNum(struct fat_dirent * dirent);
#endif
