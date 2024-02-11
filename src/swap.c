//
// Created by Hector Mejia on 1/31/24.
//

#include "swap.h"

void intSwap(int *a, int *b) {
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

void floatSwap(float *a, float *b) {
    const float tmp = *a;
    *a = *b;
    *b = tmp;
}
