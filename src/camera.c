//
// Created by Hector Mejia on 12/24/23.
//

#include "camera.h"

Camera camera = {
    .position = {
        .x = 0,
        .y = 0,
        .z = 0,
    },
    .direction = {0, 0, 1},
    .forwardVelocity = {0, 0, 0},
    .yawAngle = 0,
    .pitchAngle = 0,
};
