//
// Created by Hector Mejia on 12/24/23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct {
    Vec3 position;
    Vec3 rotation;
    float fovAngle;
} Camera;

#endif //CAMERA_H
