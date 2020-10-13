#ifndef __BINSEARCH_H__
#define __BINSEARCH_H__

#include "common.h"

#define T int 

typedef bool (*binsearch_t)(T *arr, long size, T x);

bool binsearch0(T *arr, long size, T x);

void linearize(T *dst, T *src, long size);
bool binsearch1(T *arr, long size, T x);

#endif /* !__BINSEARCH_H__ */
