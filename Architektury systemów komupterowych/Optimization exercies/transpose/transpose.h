#ifndef __TRANSPOSE_H__
#define __TRANSPOSE_H__

#include "common.h"

/* You can modify following definitions to try out different settings. */ 
#define T int 
#define BLOCK 8

typedef void (*transpose_t)(T *dst, T *src, int n);

void transpose0(T *dst, T *src, int n);
void transpose1(T *dst, T *src, int n);

#endif /* !__TRANSPOSE_H__ */
