//
// Created by Hector Mejia on 12/26/23.
//

#ifndef MESH_H
#define MESH_H

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES 6 * 2 // 6 cube faces, 2 triangles per face

#include "triangle.h"
#include "vector.h"

extern Vec3 cubeVertices[N_CUBE_VERTICES];
extern Face cubeFaces[N_CUBE_FACES];

typedef struct {
    Vec3* vertices;
    Face* faces;
    Vec3 rotation;
} Mesh;

extern Mesh mesh;

void loadCubeMeshData(void);
void freeMesh(void);

#endif //MESH_H
