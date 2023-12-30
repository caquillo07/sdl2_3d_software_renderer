#include "mesh.h"

#include <stddef.h>
#include <stdlib.h>

#include "array.h"
#include "vector.h"

Vec3 cubeVertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1}   // 8
};

Face cubeFaces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4}
};

Mesh mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {.x = 0, .y = 0, .z = 0},
};

void loadCubeMeshData(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        array_push(mesh.vertices, cubeVertices[i]);
    }
    for (int i = 0; i < N_CUBE_FACES; i++) {
        array_push(mesh.faces, cubeFaces[i]);
    }

    mesh.rotation.x = 0;
    mesh.rotation.y = 0;
    mesh.rotation.z = 0;
}

void freeMesh(void) {
    array_free(mesh.faces);
    array_free(mesh.vertices);
}
