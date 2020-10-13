/*
 * Matrix multiplication with and without blocking.
 *
 * Intel(R) Core(TM) i5-7300HQ CPU @ 2.50GHz
 *
 * $ ./matmult -n 1024 -v 0
 * Time elapsed: 5.834760 seconds.
 * $ ./matmult -n 1024 -v 1
 * Time elapsed: 0.798876 seconds.
 * $ ./matmult -n 1024 -v 2
 * Time elapsed: 13.517745 seconds.
 * $ ./matmult -n 1024 -v 3
 * Time elapsed: 0.712694 seconds.
 */
#include "matmult.h"

/* Useful macro for accessing row-major 2D arrays of size n×n. */
#define M(a, i, j) a[(i) * n + (j)]

/* ijk (& jik) */
void matmult0(int n, T_p a, T_p b, T_p c) {
    long sum;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum = 0;
            for (int k = 0; k < n; k++) {
                sum += M(a, i, k) * M(b, k, j);
            }
            M(c, i, j) = sum;
        }
    }
}

/* kij (& ikj) */
void matmult1(int n, T_p a, T_p b, T_p c) {
    long r;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            r = M(a, i, k);
            for (int j = 0; j < n; j++) {
                M(c, i, j) += r * M(b, k, j);
            }
        }
    }
}

/* jki (& kji) */
void matmult2(int n, T_p a, T_p b, T_p c) {
    long r;
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            r = M(b, k, j);
            for (int i = 0; i < n; i++) {
                M(c, i, j) += r * M(a, i, k);
            }
        }
    }
}

/* BLOCK*BLOCK tiled version */
void matmult3(int n, T_p a, T_p b, T_p c) {
    int B = BLOCK;
    for (int i = 0; i < n; i += B)
        for (int j = 0; j < n; j += B)
            for (int k = 0; k < n; k += B)
                for (int i1 = i; i1 < i + B; i1++)
                    for (int j1 = j; j1 < j + B; j1++)
                        for (int k1 = k; k1 < k + B; k1++)
                            M(c, i1, j1) += M(a, i1, k1) * M(b, k1, j1);
}
