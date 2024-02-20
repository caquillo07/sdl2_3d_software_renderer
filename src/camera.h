//
// Created by Hector Mejia on 12/24/23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct {
    Vec3 position;
    Vec3 direction;
    Vec3 forwardVelocity;
    float yawAngle;
    float pitchAngle;
} Camera;

extern Camera camera;

#endif //CAMERA_H
