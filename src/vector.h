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

Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec2 vec2_sub(Vec2 v1, Vec2 v2);
Vec2 vec2_mul(Vec2 v1, float factor);
Vec2 vec2_div(Vec2 v1, float factor);
float vec2_dot(Vec2 v1, Vec2 v2);
float vec2_length(Vec2 v);

Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_sub(Vec3 v1, Vec3 v2);
Vec3 vec3_mul(Vec3 v1, float factor);
Vec3 vec3_div(Vec3 v1, float factor);
Vec3 vec3_cross(Vec3 v1, Vec3 v2);
float vec3_dot(Vec3 v1, Vec3 v2);
float vec3_length(Vec3 v);

Vec3 vec3_rotateX(Vec3 v, float angle);
Vec3 vec3_rotateY(Vec3 v, float angle);
Vec3 vec3_rotateZ(Vec3 v, float angle);

#endif //VECTOR_H
