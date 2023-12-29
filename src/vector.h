//
// Created by Hector Mejia on 12/24/23.
//

#ifndef VECTOR_H
#define VECTOR_H

typedef struct vec2 { // todo - research the difference of adding the type before and after the struct keyword
    float x;
    float y;
} vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

// todo add funcs to manipulate vec2 and vec3
vec2 vec2_add(vec2 v1, vec2 v2);

vec3 vec3_rotate_x(vec3 v, float angle);
vec3 vec3_rotate_y(vec3 v, float angle);
vec3 vec3_rotate_z(vec3 v, float angle);

#endif //VECTOR_H
