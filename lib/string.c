asm(".code16gcc");
#include "types.h"
#include "string.h"

size_t strlen(const char * str){
	int ret = 0;
	while(1){
		if(*(str++) == NULL){
			break;
		}
		++ret;
	}
	return ret;
}

size_t  strnlen (const char *str, size_t maxlen)  
{  
	const char *char_ptr, *end_ptr = str + maxlen;  
	const unsigned long int *longword_ptr;  
	unsigned long int longword, magic_bits, himagic, lomagic;  

	if (maxlen == 0)  
		return 0;  

	if (__builtin_expect (end_ptr < str, 0))  
		end_ptr = (const char *) ~0UL;  

	for (char_ptr = str; ((unsigned long int) char_ptr  
				& (sizeof (longword) - 1)) != 0;  
			++char_ptr)  
		if (*char_ptr == 0)  
		{  
			if (char_ptr > end_ptr)  
				char_ptr = end_ptr;  
			return char_ptr - str;  
		}  

	/* All these elucidatory comments refer to 4-byte longwords, 
	   but the theory applies equally well to 8-byte longwords.  */  

	longword_ptr = (unsigned long int *) char_ptr;  

	/* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits 
	   the "holes."  Note that there is a hole just to the left of 
	   each byte, with an extra at the end: 

bits:  01111110 11111110 11111110 11111111 
bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD 

The 1-bits make sure that carries propagate to the next 0-bit. 
The 0-bits provide holes for carries to fall into.  */  
	magic_bits = 0x7efefeffL;  
	himagic = 0x80808080L;  
	lomagic = 0x01010101L;  
	if (sizeof (longword) > 4)  
	{  
		/* 64-bit version of the magic.  */  
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */  
		magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;  
		himagic = ((himagic << 16) << 16) | himagic;  
		lomagic = ((lomagic << 16) << 16) | lomagic;  
	}  
	if (sizeof (longword) > 8)  
		return -1  ;

	/* Instead of the traditional loop which tests each character, 
	   we will test a longword at a time.  The tricky part is testing 
	   if *any of the four* bytes in the longword in question are zero.  */  
	while (longword_ptr < (unsigned long int *) end_ptr)  
	{  

		if ((longword - lomagic) & himagic)  
		{  
			/* Which of the bytes was the zero?  If none of them were, it was 
			   a misfire; continue the search.  */  

			const char *cp = (const char *) (longword_ptr - 1);  

			char_ptr = cp;  
			if (cp[0] == 0)  
				break;  
			char_ptr = cp + 1;  
			if (cp[1] == 0)  
				break;  
			char_ptr = cp + 2;  
			if (cp[2] == 0)  
				break;  
			char_ptr = cp + 3;  
			if (cp[3] == 0)  
				break;  
			if (sizeof (longword) > 4)  
			{  
				char_ptr = cp + 4;  
				if (cp[4] == 0)  
					break;  
				char_ptr = cp + 5;  
				if (cp[5] == 0)  
					break;  
				char_ptr = cp + 6;  
				if (cp[6] == 0)  
					break;  
				char_ptr = cp + 7;  
				if (cp[7] == 0)  
					break;  
			}  
		}  
		char_ptr = end_ptr;  
	}  

	if (char_ptr > end_ptr)  
		char_ptr = end_ptr;  
	return char_ptr - str;  
}

void tolower(char * str) 
{
	int len = strlen(str);
	int i = 0;
	for( ; i<len ; i++){
		if('A' <= str[i] && str[i] <= 'a'){
			str[i] -= 'A' + 'a'; 
		}
	}
}

int tolowerchar(char c){
	if('A' <= c && c <= 'a'){
		return c - 'A' + 'a';
	}
	return c;
}
int strcasecmp(const char *s1,const char *s2)  
{  
	return strncasecmp(s1,s2,strlen(s1));
}

int strncasecmp(const char *s1,const char *s2,int n){
	int c1, c2;  
	int len = n;
	int i = 0;
	for(;i<len;i++){
		c1 = tolowerchar(s1[i]);
		c2 = tolowerchar(s2[i]);
		if(c1 == c2 ){
			continue;
		}else{
			break;
		}
	}
	return c1 -c2;
}

void trimCrlf(char *sLine)
{
	int n;
	n=strlen(sLine)-1;
	while (n>0 && (sLine[n]=='\r' ||  sLine[n]=='\n'))
	{   
		sLine[n]=0;n--;
	}   
}

void trimRight(char *sLine, int nLength){
	int n,i;
	n = strlen(sLine)-1;
	n = (n<nLength)? n : nLength;
	i = n;
	if( i>0 )
	{
		while( i>=0 && (sLine[i]=='\t' || sLine[i]==' ' || sLine[i]=='\n' || sLine[i]=='\r'))
		{
			i--;
		}
		sLine[i+1]=0;
	}
	return;
}
void trimQuotes(char *sLine, int nLength){
	trimLeft(sLine,nLength);
	trimRight(sLine,nLength);
	if(sLine[0] == '\"' || sLine[0] == '\''){
		int i=0;
		for( ;i < nLength - 1 ;i++){
			sLine[i] = sLine[i + 1];
		}
		sLine[nLength - 1] = 0;
	}
	int lineLength = strlen(sLine) - 1;
	if(sLine[lineLength]== '\"' || sLine[lineLength] == '\'' ){
		sLine[lineLength] = 0;
	}
	return;

}

void trimLeft(char *sLine, int nLength){
	int n,i,j;
	n = strlen(sLine);
	n = (n<nLength)? n : nLength;
	i = 0;
	if( i<n && n>0 )
	{
		while( i<n && (sLine[i]=='\t' || sLine[i]==' '))
		{
			i++;
		}
		if( i>0 )
		{
			j = 0;
			while( j<=n-i )
			{
				sLine[j] = sLine[j+i];
				j++;
			}
			sLine[j]=0;
		}
	}
	return;
}
void trim(char *sLine,int nLength){
	trimLeft(sLine,nLength);
	trimRight(sLine,nLength);
}
char *strcpy(char *dst,const char *src)  
{  
	if((dst==NULL)||(src==NULL)) return NULL;
	char *ret = dst;  
	while((* dst++ = * src++) != '\0')   
		;  
	return ret;  
}

char *strncpy ( char * dest, const char * source, int count )
{
	if((dest==NULL)||(source==NULL)) return NULL;
	char *start = dest;
	while (count && (*dest++ = *source++))
		count--;
	if(count){
		while (--count)
			*dest++ = ' ';
	}
	return (start);
}

char *strcat(char *str1, char *str2)  
{  
	if((str1==NULL)||(str2==NULL)) return NULL;  
	char *pt = str1;  
	while(*str1!='\0') str1++;  
	while(*str2!='\0') *str1++ = *str2++;  
	*str1 = '\0';  
	return pt;  
}  

char* strstr(const char *s1, const char *s2)
{
	int n;
	if (*s2)
	{
		while (*s1)
		{
			for (n=0; *(s1 + n) == *(s2 + n); n++)
			{
				if (!*(s2 + n + 1))
					return (char *)s1;
			}
			s1++;
		}
		return NULL;
	}
	else
		return (char *)s1;
}
