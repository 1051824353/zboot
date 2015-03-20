asm(".code16gcc");
#include "command.h"
#include "string.h"
#include "print.h"
/*
 * search:查询内容
 * return:
 *  1 	为切换页面正常
 *  -1 	为切换目录不正常
 *	-2 	为目录不存在
 */
int syscd(filesystem * fs,char * search){
	if(search == NULL){
		return -1;
	}
	int s = 0;
	if(strcasecmp(search,".") == 0){
		return 1;
	}else{
		if(strcasecmp(search,"..") == 0 ){
			s = fs->curdir;

			struct fat_dirent * dirent = (struct fat_dirent *)getDirent(fs,s);
			dirent++;
			int clusNum = getClusNum(dirent);
			if(clusNum == 0){
				fs->curdir = fs->rootdir;
			}else{
				fs->curdir = fs->rootdir + (clusNum - 2) * 8;
			}
		}else{
			filestat stat;
			searchByName(fs,search,&stat);
			if(stat.isDir == 1){
				int clusNum = stat.start_index;
				fs->curdir = fs->rootdir + (clusNum - 2) * 8; // 0x1000 / 200
			}else{
				return -2;
			}
		}
	}
	return 1;

}
int cd(filesystem * fs,char * search){
	int ret = syscd(fs,search);
	if(ret == -2){
		printf("not dir");
	}else if(ret == 1){
		printf(search);
	}else if(ret == 1){
		printf("param error");
	}

	return ret;
}
