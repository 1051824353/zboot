asm(".code16gcc");
#include "command.h"
#include "fat.h"
#include "string.h"
#include "print.h"
#include "malloc.h"

int ls(filesystem * fs,char * search){
	if(search != NULL && strlen(search) > 0){
		filestat outstat;
		searchByName(fs,search,&outstat);
		if(outstat.isDir == 1){
			printf("D ");
		}
		printf(outstat.filename);
		printf("  ");

	}else{
		filestat stat[20];
		memset(&stat,0,sizeof(filestat)*20 );
		int size = listFileStat(fs,(filestat *)&stat,fs->curdir);
		int i = 0;

		for(;i<size;i++){
			if(stat[i].isDir == 1){
				printf("D ");
			}
			printf(stat[i].filename);
			printf("  ");
		}
	}
	return 1;
}

int searchByName(filesystem * fs,char * search,filestat * outstat){
	if(search == NULL){
		return -1;
	}
	filestat stat[20];
	memset(&stat,0,sizeof(filestat) *20);
	int size = listFileStat(fs,(filestat *)&stat,fs->curdir);
	int i = 0;
	for(;i<size;i++){
		if(search !=NULL && strcasecmp(search,stat[i].filename) == 0) {
			memcpy(outstat,&stat[i],sizeof(filestat));
			return 1;
		}
	}
	return 0;
}

int searchById(filesystem * fs,int id,filestat * outstat){
	filestat stat[20];
	memset(&stat,0,sizeof(filestat) *20);
	int size = listFileStat(fs,(filestat *)&stat,fs->curdir);
	int i = 0;
	for(;i<size;i++){
		if(id == stat[i].start_index) {
			memcpy(outstat,&stat[i],sizeof(filestat));
			return 1;
		}
	}
	return 0;
}
