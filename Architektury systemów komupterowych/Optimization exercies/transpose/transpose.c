/*
 * Matrix transposition with and without blocking.
 *
 * Intel(R) Core(TM) i5-7300HQ CPU @ 2.50GHz
 *
 * $ ./transpose -n 32768 -v 0
 * Time elapsed: 9.693806 seconds.
 * $ ./transpose -n 32768 -v 1
 * Time elapsed: 1.766504 seconds.
 */
#include "transpose.h"

void transpose0(T *dst, T *src, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[j * n + i] = src[i * n + j];
}

void transpose1(T *dst, T *src, int n) {
    int B = BLOCK;
    for (int i = 0; i < n; i += B)
        for (int j = 0; j < n; j += B)
            for (int i1 = i; i1 < i + B; i1++)
                for (int j1 = j; j1 < j + B; j1++)
                    dst[j1 * n + i1] = src[i1 * n + j1];
}
