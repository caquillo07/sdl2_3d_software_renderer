//
// Created by Hector Mejia on 12/26/23.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct {
    int a, b, c;
} Face;

typedef struct {
    Vec2 points[3];
} Triangle;

#endif //TRIANGLE_H
