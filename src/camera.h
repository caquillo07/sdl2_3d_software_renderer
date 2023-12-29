//
// Created by Hector Mejia on 12/24/23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct {
    vec3 position;
    vec3 rotation;
    float fovAngle;
} camera;

#endif //CAMERA_H
