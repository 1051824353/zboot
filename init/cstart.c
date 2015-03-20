asm(".code16gcc");
#include "bios.h"
#include "print.h"
#include "common.h"
#include "string.h"
#include "fat.h"
#include "malloc.h"
#include "command.h"
#include "console.h"


void cstart(){
	char cLine[32];
	char * usage_char = "***** bootloader make by zengwei *****\n";
	char * shell_char = "command>";
	printf(usage_char);
	memset(cLine,0,32);
	filesystem * fs = NULL;
	/*init*/
	malloc_init(0xc000);
	fs = initFileSystem();
	/*init*/
getchar:	
	printf(shell_char);
	gets(cLine);
	trim(cLine,strlen(cLine));
	char * param = strstr(cLine," ");
	if(param != NULL){
		param += 1;
	}
	int len = strlen(cLine);
	if(len == 0){
		goto getchar;
	}else if(strncasecmp(cLine,"ls",2) == 0){
		ls(fs,param);
	}else if(strncasecmp(cLine,"pwd",3) == 0){
		pwd(fs);
	}else if(strncasecmp(cLine,"cd",2) == 0){
		cd(fs,param);
	}else if(strncasecmp(cLine,"iso",3) == 0){
		iso(fs,param);
	}else{
		printf("command not fonud");
	}
	printf("\n");
	goto getchar;
	loop_sleep();
	return ;
}
