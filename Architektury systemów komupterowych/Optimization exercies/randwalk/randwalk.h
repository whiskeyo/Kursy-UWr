#ifndef __RANDWALK_H__
#define __RANDWALK_H__

#include "common.h"

typedef int (*randwalk_t)(uint8_t *arr, int n, int len);

int randwalk0(uint8_t *arr, int n, int len);
int randwalk1(uint8_t *arr, int n, int len);

#endif /* !__RANDWALK_H__ */
