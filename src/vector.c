//
// Created by Hector Mejia on 12/24/23.
//

#include "vector.h"
#include "math.h"

// 2D vectors //
Vec2 vec2_add(const Vec2 v1, const Vec2 v2) {
    return (Vec2){
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
    };
}

Vec2 vec2_sub(const Vec2 v1, const Vec2 v2) {
    return (Vec2){
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
    };
}

Vec2 vec2_mul(const Vec2 v1, const float factor) {
    return (Vec2){
        .x = v1.x * factor,
        .y = v1.y * factor,
    };
}

Vec2 vec2_div(const Vec2 v1, const float factor) {
    return (Vec2){
        .x = v1.x / factor,
        .y = v1.y / factor,
    };
}

void vec2_normalize(Vec2* v) {
    const float length = sqrt(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}

float vec2_dot(const Vec2 v1, const Vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float vec2_length(const Vec2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

// 3D vectors //
Vec3 vec3_add(const Vec3 v1, const Vec3 v2) {
    return (Vec3){
        .x = v1.x + v2.x,
        .y = v1.y + v2.y,
        .z = v1.z + v2.z,
    };
}

Vec3 vec3_sub(const Vec3 v1, const Vec3 v2) {
    return (Vec3){
        .x = v1.x - v2.x,
        .y = v1.y - v2.y,
        .z = v1.z - v2.z,
    };
}

Vec3 vec3_mul(const Vec3 v1, const float factor) {
    return (Vec3){
        .x = v1.x * factor,
        .y = v1.y * factor,
        .z = v1.z * factor,
    };
}

Vec3 vec3_div(const Vec3 v1, const float factor) {
    return (Vec3){
        .x = v1.x / factor,
        .y = v1.y / factor,
        .z = v1.z / factor,
    };
}

Vec3 vec3_cross(const Vec3 v1, const Vec3 v2) {
    return (Vec3){
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v1.x * v2.z,
        .z = v1.x * v2.y - v1.y * v2.x,
    };
}

void vec3_normalize(Vec3* v) {
    const float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

float vec3_dot(const Vec3 v1, const Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec3_length(const Vec3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_rotateX(const Vec3 v, const float angle) {
    return (Vec3){
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
}

Vec3 vec3_rotateY(const Vec3 v, const float angle) {
    return (Vec3){
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
}

Vec3 vec3_rotateZ(const Vec3 v, const float angle) {
    return (Vec3){
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
}

Vec4 vec4_from_vec3(Vec3 v) {
    Vec4 result = { v.x, v.y, v.z, 1.0f };
    return result;
}

Vec3 vec3_from_vec4(Vec4 v) {
    Vec3 result = { v.x, v.y, v.z };
    return result;
}
