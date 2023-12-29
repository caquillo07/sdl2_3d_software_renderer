//
// Created by Hector Mejia on 12/24/23.
//

#include "vector.h"
#include "math.h"

vec2 vec2_add(const vec2 v1, const vec2 v2) {
    return (vec2){v1.x + v2.x, v1.y + v2.y};
}

vec3 vec3_rotate_x(const vec3 v, const float angle) {
    return (vec3){
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
}

vec3 vec3_rotate_y(const vec3 v, const float angle) {
    return (vec3){
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
}

vec3 vec3_rotate_z(const vec3 v, const float angle) {
    return (vec3){
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
}
