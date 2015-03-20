#ifndef COMMON_H
#define COMMON_H

#include "types.h"

typedef char * va_list;
 
#define _INTSIZEOF(n) \
	((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )
 
#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
 
#define va_arg(ap,t) \
	( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
 
#define va_end(ap) ( ap = (va_list)0 )

int isdigit( int ch );
void sleep(long sec);
void loop_sleep();

static inline unsigned short read16(le16_t * _p)
{
    return *((const uint16_t *)_p);
}
static inline unsigned char read8(le8_t * _p)
{
    return *_p;
}

static inline void write8(le8_t * _p, uint8_t _v)
{
    *_p = _v;
}

static inline void write16(le16_t * _p, unsigned short _v)
{
    *((uint16_t *) _p) = _v;
}

static inline unsigned int read32(le32_t * _p)
{
    return *((const uint32_t *)_p);
}

static inline void write32(le32_t * _p, uint32_t _v)
{
    *((uint32_t *) _p) = _v;
}


#endif
