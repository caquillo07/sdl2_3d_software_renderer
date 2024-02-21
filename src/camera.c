//
// Created by Hector Mejia on 12/24/23.
//

#include "camera.h"
#include "matrix.h"

static Camera camera = {
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

void initCamera(Vec3 position, Vec3 direction) {
    camera.position = position;
    camera.direction = direction;
    camera.forwardVelocity = vec3_new(0, 0, 0);
    camera.yawAngle = 0.0;
    camera.pitchAngle = 0.0;
};

Vec3 getCameraPosition(void) {
    return camera.position;
}

Vec3 getCameraDirection(void) {
    return camera.direction;
}

Vec3 getCameraForwardVelocity(void) {
    return camera.forwardVelocity;
}

float getCameraYaw(void) {
    return camera.yawAngle;
}

float getCameraPitch(void) {
    return camera.pitchAngle;
}

void updateCameraPosition(Vec3 position) {
    camera.position = position;
}

void updateCameraDirection(Vec3 direction) {
    camera.direction = direction;
}

void updateCameraForwardVelocity(Vec3 forwardVelocity) {
    camera.forwardVelocity = forwardVelocity;
}

void rotateCameraYaw(float angle) {
    camera.yawAngle += angle;
}

void rotateCameraPitch(float angle) {
    camera.pitchAngle += angle;
}

Vec3 getCameraLookAtTarget(void) {
    // Initialize the target looking at the positive z-axis
    Vec3 target = { 0, 0, 1 };

    Mat4 cameraYawRotation = mat4_makeRotationY(camera.yawAngle);
    Mat4 cameraPitchRotation = mat4_makeRotationX(camera.pitchAngle);

    // Create camera rotation matrix based on yaw and pitch
    Mat4 cameraRotation = mat4_identity();
    cameraRotation = mat4_mulMat4(cameraPitchRotation, cameraRotation);
    cameraRotation = mat4_mulMat4(cameraYawRotation, cameraRotation);

    // Update camera direction based on the rotation
    Vec4 cameraDirection = mat4_mulVec4(cameraRotation, vec4_fromVec3(target));
    camera.direction = vec3_fromVec4(cameraDirection);

    // Offset the camera position in the direction where the camera is pointing at
    target = vec3_add(camera.position, camera.direction);

    return target;
}

