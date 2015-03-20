#include "types.h"


void malloc_init (ulong dest_addr);
void free(void *ap);
void* malloc (unsigned n);
void memset (void *p, int c, size_t v);
void *calloc(size_t n, size_t size);
void *memcpy(void *dst,const void *src,size_t num); 
