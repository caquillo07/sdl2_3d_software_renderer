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

void initCamera(Vec3 position, Vec3 direction);

Vec3 getCameraPosition(void);
Vec3 getCameraDirection(void);
Vec3 getCameraForwardVelocity(void);
float getCameraYaw(void);
float getCameraPitch(void);

void updateCameraPosition(Vec3 position);
void updateCameraDirection(Vec3 direction);
void updateCameraForwardVelocity(Vec3 forwardVelocity);

void rotateCameraYaw(float angle);
void rotateCameraPitch(float angle);

Vec3 getCameraLookAtTarget(void);


#endif //CAMERA_H
