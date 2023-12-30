//
// Created by Hector Mejia on 12/24/23.
//

#ifndef VECTOR_H
#define VECTOR_H

typedef struct vec2 { // todo - research the difference of adding the type before and after the struct keyword
    float x;
    float y;
} Vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
} Vec3;

// todo add funcs to manipulate vec2 and vec3
Vec2 vec2_add(Vec2 v1, Vec2 v2);

Vec3 vec3_rotate_x(Vec3 v, float angle);
Vec3 vec3_rotate_y(Vec3 v, float angle);
Vec3 vec3_rotate_z(Vec3 v, float angle);

#endif //VECTOR_H
