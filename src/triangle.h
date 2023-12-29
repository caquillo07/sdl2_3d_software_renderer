//
// Created by Hector Mejia on 12/26/23.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct {
    int a, b, c;
} face;

typedef struct {
    vec2 points[3];
} triangle;

#endif //TRIANGLE_H
