asm(".code16gcc");
#include "types.h"

int isdigit( int ch ){
	return (unsigned int)(ch - '0') < 10u;
}

void sleep(long sec){
	long stime = sec *1000;
	while(--stime);
}

void loop_sleep(){
	while(1);
}
unsigned char read8(le8_t * _p)
{
    return *_p;
}

void write8(le8_t * _p, uint8_t _v)
{
    *_p = _v;
}

void write16(le16_t * _p, unsigned short _v)
{
    *((uint16_t *) _p) = _v;
}

unsigned int read32(le32_t * _p)
{
    return *((const uint32_t *)_p);
}

void write32(le32_t * _p, uint32_t _v)
{
    *((uint32_t *) _p) = _v;
}


