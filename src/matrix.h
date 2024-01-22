//
// Created by Hector Mejia on 1/17/24.
//

#ifndef SDL2_SOFTWARE_RENDERER_MATRIX_H
#define SDL2_SOFTWARE_RENDERER_MATRIX_H

#include "vector.h"

typedef struct {
    float m[4][4];
} Mat4;

Mat4 mat4_identity(void);
Mat4 mat4_makeScale(float sx, float sy, float sz);
Mat4 mat4_makeTransform(float tx, float ty, float tz);
Mat4 mat4_makeRotationX(float rx);
Mat4 mat4_makeRotationY(float angle);
Mat4 mat4_makeRotationZ(float rz);
Mat4 mat4_makeWorld(Vec3 position, Vec3 rotation, Vec3 scale);
Mat4 mat4_makePerspective(float fov, float aspect, float znear, float zfar);
Vec4 mat4_mulVec4(Mat4 m, Vec4 v);
Mat4 mat4_mulMat4(Mat4 m1, Mat4 m2);
Vec4 mat4_mulVec4Project(Mat4 m, Vec4 v);

#endif //SDL2_SOFTWARE_RENDERER_MATRIX_H
