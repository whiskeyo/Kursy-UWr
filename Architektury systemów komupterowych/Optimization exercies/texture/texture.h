#ifndef __RANDWALK_H__
#define __RANDWALK_H__

#include "common.h"

/* You can modify following definitions to try out different settings. */ 
#define N 8192
typedef uint8_t T;

typedef void (*fill_t)(T *);
typedef long (*query_t)(T *, long, long, long, long, long *);

void fill_0(T *arr);
long query_0(T *arr, long xs, long ys, long xe, long ye, long *sum_p);

void fill_1(T *arr);
long query_1(T *arr, long xs, long ys, long xe, long ye, long *sum_p);

#endif /* !__RANDWALK_H__ */
