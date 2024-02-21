//
// Created by Hector Mejia on 2/11/24.
//

#ifndef SDL2_SOFTWARE_RENDERER_CLIPPING_H
#define SDL2_SOFTWARE_RENDERER_CLIPPING_H

#include "vector.h"
#include "triangle.h"

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLES 10

typedef enum {
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE,
    NUM_FRUSTUM_PLANES
} FrustumPlane;

typedef struct {
    Vec3 point;
    Vec3 normal;
} Plane;

typedef struct {
    Vec3 vertices[MAX_NUM_POLY_VERTICES];
    Texture2 textCoords[MAX_NUM_POLY_VERTICES];
    int numVertices;
} Polygon;

void initFrustumPlanes(float fovX, float fovY, float zNear, float zFar);

Polygon createPolygonFromTriangle(
    Vec3 v0,
    Vec3 v1,
    Vec3 v2,
    const Texture2 t0,
    const Texture2 t1,
    const Texture2 t2
);

void clipPolygon(Polygon *polygon);

void createTrianglesFromPolygon(Polygon *polygon, Triangle triangles[], int *numTriangles);

//void floatLerp(float u, float u1, float t);

#endif //SDL2_SOFTWARE_RENDERER_CLIPPING_H
