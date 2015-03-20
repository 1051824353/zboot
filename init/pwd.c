asm(".code16gcc");
#include "command.h"
#include "string.h"
#include "malloc.h"
#include "print.h"

int pwd(filesystem * fs){
	int curdir = fs->curdir;
	filestat stat;
	char filePath[256];
	char tfilePath[256];
	memset(filePath,0,256);
	memset(tfilePath,0,256);
	while(1){
		if(searchByName(fs,".",&stat) == 0){
			break;
		}
		syscd(fs,"..");
		int statIndex = stat.start_index;
		if(searchById(fs,statIndex,&stat) == 0){
			break;
		}
		strcpy(tfilePath,filePath);
		strcpy(filePath,"/");
		strcat(filePath,stat.filename);
		strcat(filePath,tfilePath);
	}
	printf(filePath);
	fs->curdir = curdir;
	return 1;
}
