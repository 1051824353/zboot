#ifndef LIBSTRING_H
#define LIBSTRING_H


#include "types.h"


size_t strlen(const char * str);
size_t strnlen (const char *str, size_t maxlen) ; 
int strcasecmp(const char *s1, const char *s2); 
int strncasecmp(const char *s1,const char *s2,int n);
void trim(char *sLine,int nLength);
void trimLeft(char *sLine, int nLength);
void trimRight(char *sLine, int nLength);
char *strcpy(char *dst,const char *src); 
char *strncpy ( char * dest, const char * source, int count );
char *strcat(char *str1, char *str2); 
char *strstr(const char *s1, const char *s2);

#endif
