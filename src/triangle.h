//
// Created by Hector Mejia on 12/26/23.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include "vector.h"

typedef struct {
    int a, b, c;
} Face;

typedef struct {
    Vec2 points[3];
} Triangle;

void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif //TRIANGLE_H
