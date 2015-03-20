asm(".code16gcc");
#include "command.h"
#include "string.h"
#include "print.h"
#include "malloc.h"

extern ulong mem_malloc_brk;

void (*theboot)(void);

int iso(filesystem * fs,char * search){
	filestat outstat;
	searchByName(fs,search,&outstat);
	void * cdiso = (void *)malloc(outstat.size);
	int sector = outstat.size / LIBFAT_SECTOR_SIZE;
	int ret = xpread( cdiso, LIBFAT_SECTOR_SIZE,sector);
	if(ret == 0){
		printf("read mem error");
		return -1;
	}else if(ret > 0){
		printf("read mem ok");
	}
	theboot = (void (*)(void))(cdiso);
	theboot();
	return 1;
}
