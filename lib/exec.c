asm(".code16gcc");
#include "types.h"
#include "string.h"
#include "fat.h"

int execvp(filesystem *fs,const char *file ,char * const argv []){
	file_stat stat;
	searchClusFile(fs,file,&stat);

}
