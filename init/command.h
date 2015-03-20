#include "fat.h"
int ls(filesystem * fs,char * search);
int cd(filesystem * fs,char * search);
int syscd(filesystem * fs,char * search);
int pwd(filesystem * fs);
int searchById(filesystem * fs,int id,filestat * outstat);
int searchByName(filesystem * fs,char * search,filestat * outstat);
int iso(filesystem * fs,char * search);
