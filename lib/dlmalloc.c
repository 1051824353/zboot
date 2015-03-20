asm(".code16gcc");
#include "malloc.h"

#define MINIMUM 1024  /*通过sbrk分配的最小内存 */

typedef struct header Header;
struct header {
	struct header* next; /* 指向下一个节点的指针 */
	unsigned size;
};

//static Header base; /* 链表头 */
static Header* freep = NULL; /* 空闲内存的链表 */


static ulong mem_malloc_start = 0;
static ulong mem_malloc_end = 0;
static ulong mem_malloc_brk = 0;

void malloc_init (ulong dest_addr){
	mem_malloc_start = dest_addr;
   	mem_malloc_end = dest_addr + 0x8000;
    mem_malloc_brk = mem_malloc_start;
}

void *sbrk (ptrdiff_t increment)
{
	ulong old = mem_malloc_brk;
	ulong new = old ;

	if ((new < mem_malloc_start) || (new > mem_malloc_end)) {
		return (NULL);
	}
	mem_malloc_brk += increment;

	return ((void *) old);
}

/* 从内核中请求更多的内存 */
Header* moremem (unsigned n) 
{
	char* p;
	Header* up;

	if (n < MINIMUM)
		n = MINIMUM;

	p = (char *)sbrk(n * sizeof(Header));

	if (p == (char *) -1) /* 没有空闲内存 */
		return NULL;

	up = (Header *) p;
	up->size = n;
	free((void *)(up + 1));

	return freep;
}

/*free: 将块ap放入空闲块列表中*/
void free(void *ap)
{
	Header *bp, *p;
	bp = (Header *)ap - 1; /*指向块头*/

	for (p = freep; !(bp > p && bp < p->next); p = p->next)
		if (p >= p->next && (bp > p || bp < p->next))
			break; /*块freep在列表的开始或末尾*/
	if (bp + bp->size == p->next) { /*与上一相邻块合并*/
		bp->size += p->next->size;
		bp->next = p->next->next;
	} else
		bp->next = p->next;
	if (p + p->size == bp) { /*与下一相邻块合并*/
		p->size += bp->size;
		p->next = bp->next;
	} else
		p->next = bp;
	freep = p;
}

void* malloc(unsigned n){
	char * p = (char *)sbrk(n);
	return (void*)p;
}
/*
void* malloc (unsigned n) 
{
	Header* p, *prev;
	unsigned nunits;

	nunits = (n + sizeof(Header) - 1) / sizeof(Header);
	if ((prev = freep) == NULL) {
		base.next = freep = prev = &base;
		base.size = 0;
	}

	for (p = prev->next; ; prev = p, p = p->next) {
		if (p->size >= nunits) { 
			if (p->size == nunits) 
				prev->next = p->next;
			else { 
				p->size -= nunits;
				p += p->size;
				p->size = nunits;
			}
			freep = prev;
			return (void *)(p + 1);
		}

		if (p == freep) 
			if ((p = moremem(nunits)) == NULL)
				return NULL; 
	}
}*/
void memset (void *p, int c, size_t v)
{
	char * tmp = p;
	while (v--){
		*(tmp++) = c;
	}
	return;
}
void *calloc(size_t n, size_t size){
	return malloc(n*size);
}

void *memcpy(void *dst,const void *src,size_t num)  
{  
	byte * psrc = (byte *)src;//byte 既为unsigned char类型  
	byte * pdst = (byte *)dst;  
	while(num-->0)*pdst++ = *psrc++;  
	return dst;  
}  
