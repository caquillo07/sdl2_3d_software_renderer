//
// Created by Hector Mejia on 12/26/23.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include "vector.h"
#include "texture.h"

typedef struct Face {
    int a, b, c;
    uint32_t color;
    Texture2 vertexA_UV;
    Texture2 vertexB_UV;
    Texture2 vertexC_UV;
} Face;

typedef struct Triangle {
    Vec4 points[3];
    Texture2 textCoords[3];
    uint32_t color;
} Triangle;

void drawFilledTriangle(
    int x0, int y0, float z0, float w0,
    int x1, int y1, float z1, float w1,
    int x2, int y2, float z2, float w2,
    uint32_t color
);

void drawTrianglePixel(
    int x, int y, uint32_t color,
    Vec4 pointA, Vec4 pointB, Vec4 pointC
);

void drawTexel(
    int x, int y, const uint32_t *texture,
    Vec4 pointA, Vec4 pointB, Vec4 pointC,
    Texture2 vertexA_UV, Texture2 vertexB_UV, Texture2 vertexC_UV
);

void drawTexturedTriangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    uint32_t *texture
);

Vec3 barycentricWeights(Vec2 a, Vec2 b, Vec2 c, Vec2 p);

#endif //TRIANGLE_H
