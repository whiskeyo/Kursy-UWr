/*
 * Binary search with linearly placed tree levels.
 *
 * Intel® Core™ i5-7300HQ CPU @ 2.50GHz
 *
 * $ ./binsearch -S 0x5bab3de5da7882ff -n 23 -t 24 -v 0
 * Time elapsed: 7.616777 seconds.
 * $ ./binsearch -S 0x5bab3de5da7882ff -n 23 -t 24 -v 1
 * Time elapsed: 2.884369 seconds.
 */
#include "binsearch.h"

bool binsearch0(T *arr, long size, T x)
{
    do
    {
        size >>= 1;
        T y = arr[size];
        if (y == x)
            return true;
        if (y < x)
            arr += size + 1;
    } while (size > 0);
    return false;
}

// Dwie ponizsze procedury odnosza sie do kopca, a wiec
// linearize to to samo co heapify, a binsearch1 to
// wyszukiwanie w kopcu.
void linearize(T *dst, T *src, long size)
{
    long i = 0, index = 0;
    long offset, removed_count, elements_count;
    long start_index, middle_index, end_index;

    // tworzenie kolejnych poziomow kopca
    while ((1 << i) < size)
    {
        offset = (size - 1) >> i;
        removed_count = 0;

        for (elements_count = 0; elements_count < (1 << i); elements_count++)
        {
            start_index = elements_count * offset + removed_count;
            end_index = (elements_count + 1) * offset + removed_count;
            middle_index = (start_index + end_index) >> 1;

            removed_count++;
            dst[index++] = src[middle_index];
        }

        i++;
    }
}

bool binsearch1(T *arr, long size, T x)
{
    long i = 1;

    do
    {
        int y = arr[i - 1];
        long j = i * 2;

        j = j | (x > y);
        i = j;

        if (x == y)
            return true;
    } while (i <= size);

    return false;
}
