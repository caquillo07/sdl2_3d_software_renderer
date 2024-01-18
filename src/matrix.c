//
// Created by Hector Mejia on 1/17/24.
//

#include <math.h>
#include "matrix.h"
#include "vector.h"

Mat4 mat4_identity(void) {
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |
    Mat4 result = {
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}

Mat4 mat4_makeScale(float sx, float sy, float sz) {
    // | sx  0  0  0 |
    // |  0 sy  0  0 |
    // |  0  0 sz  0 |
    // |  0  0  0  1 |
    Mat4 m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    m.m[3][3] = 1.0f;
    return m;
}

Mat4 mat4_makeTransform(float tx, float ty, float tz) {
    // | 1 0 0 tx |
    // | 0 1 0 ty |
    // | 0 0 1 tz |
    // | 0 0 0  1 |
    Mat4 m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

Vec4 mat4_mulVec4(Mat4 m, Vec4 v) {
    Vec4 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

Mat4 mat4_makeRotationX(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Mat4 result = mat4_identity();
    // | 1       0       0 0 |
    // | 0 cos(angle) -sin(angle) 0 |
    // | 0 sin(angle)  cos(rx) 0 |
    // | 0       0       0 1 |
    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;
    return result;
}

Mat4 mat4_makeRotationY(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    Mat4 result = mat4_identity();
    // | cos(ry) 0 sin(ry) 0 |
    // |       0 1       0 0 |
    // | -sin(ry) 0 cos(ry) 0 |
    // |       0 0       0 1 |
    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;
    return result;
}

Mat4 mat4_makeRotationZ(float rz) {
    Mat4 result = mat4_identity();
    float c = cosf(rz);
    float s = sinf(rz);

    // | c -s  0  0 |
    // | s  c  0  0 |
    // | 0  0  1  0 |
    // | 0  0  0  1 |
    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;
    return result;
}

Mat4 mat4_makeWorld(Vec3 position, Vec3 rotation, Vec3 scale) {
    Mat4 result = mat4_identity();
    Mat4 scaleMatrix = mat4_makeScale(scale.x, scale.y, scale.z);
    Mat4 translationMatrix = mat4_makeTransform(position.x, position.y, position.z);
    Mat4 rotationMatrixX = mat4_makeRotationX(rotation.x);
    Mat4 rotationMatrixY = mat4_makeRotationY(rotation.y);
    Mat4 rotationMatrixZ = mat4_makeRotationZ(rotation.z);

    // world = scale * rotation * translation
    // order matters on this one.
    result = mat4_mulMat4(scaleMatrix, result);
    result = mat4_mulMat4(rotationMatrixX, result);
    result = mat4_mulMat4(rotationMatrixY, result);
    result = mat4_mulMat4(rotationMatrixZ, result);
    result = mat4_mulMat4(translationMatrix, result);

    return result;
}

Mat4 mat4_mulMat4(Mat4 m1, Mat4 m2) {
    Mat4 result = mat4_identity();
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4 ; col++) {
            result.m[row][col] = m1.m[row][0] * m2.m[0][col] +
                               m1.m[row][1] * m2.m[1][col] +
                               m1.m[row][2] * m2.m[2][col] +
                               m1.m[row][3] * m2.m[3][col];
        }
    }
    return result;
}
