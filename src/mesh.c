#include "mesh.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    {.a = 1, .b = 2, .c = 3, .color = 0xFFFFFFFF},
    {.a = 1, .b = 3, .c = 4, .color = 0xFFFFFFFF},
    // right
    {.a = 4, .b = 3, .c = 5, .color = 0xFFFFFFFF},
    {.a = 4, .b = 5, .c = 6, .color = 0xFFFFFFFF},
    // back
    {.a = 6, .b = 5, .c = 7, .color = 0x0FFFFFFFF},
    {.a = 6, .b = 7, .c = 8, .color = 0x0FFFFFFFF},
    // left
    {.a = 8, .b = 7, .c = 2, .color = 0xFFFFFFFF},
    {.a = 8, .b = 2, .c = 1, .color = 0xFFFFFFFF},
    // top
    {.a = 2, .b = 7, .c = 5, .color = 0xFFFFFFFF},
    {.a = 2, .b = 5, .c = 3, .color = 0xFFFFFFFF},
    // bottom
    {.a = 6, .b = 8, .c = 1, .color = 0xFFFFFFFF},
    {.a = 6, .b = 1, .c = 4, .color = 0xFFFFFFFF}
};

Mesh mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {.x = 0, .y = 0, .z = 0},
    .scale = {.x = 1.0f, .y = 1.0f, .z = 1.0f},
    .translation = {.x = 0, .y = 0, .z = 0},
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

void loadOBJFileData(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", fileName);
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh.vertices, vertex);
        } else if (strncmp(line, "f ", 2) == 0) {
            int vertexIndices[3];
            int textureIndices[3];
            int normalIndices[3];
            sscanf(
                line,
                "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertexIndices[0], &textureIndices[0], &normalIndices[0],
                &vertexIndices[1], &textureIndices[1], &normalIndices[1],
                &vertexIndices[2], &textureIndices[2], &normalIndices[2]
            );
            const Face face = {
                .a = vertexIndices[0],
                .b = vertexIndices[1],
                .c = vertexIndices[2],
                .color = 0xFFFFFFFF,
            };
            array_push(mesh.faces, face);
        }
    }

    fclose(file);
}

void freeMesh(void) {
    array_free(mesh.faces);
    array_free(mesh.vertices);
}
