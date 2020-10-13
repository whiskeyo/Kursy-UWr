#ifndef __MATMULT_H__
#define __MATMULT_H__

#include "common.h"

#define T long

/* You may modify following definition to try out different settings. */ 
#define BLOCK 16

/* Break matrices start alignment with following offsets: */
#define A_OFFSET NITEMS(BLOCK_SIZE * 2, T)
#define B_OFFSET NITEMS(BLOCK_SIZE * 1, T)
#define C_OFFSET NITEMS(BLOCK_SIZE * 0, T)

typedef T *restrict T_p;
typedef void (*matmult_t)(int n, T_p a, T_p b, T_p c);

void matmult0(int n, T_p a, T_p b, T_p c);
void matmult1(int n, T_p a, T_p b, T_p c);
void matmult2(int n, T_p a, T_p b, T_p c);
void matmult3(int n, T_p a, T_p b, T_p c);

#endif /* !__MATMULT_H__ */
