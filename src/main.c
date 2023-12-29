#include <stdbool.h>
#include <SDL2/SDL.h>

#include "camera.h"
#include "display.h"
#include "mesh.h"
#include "vector.h"

triangle trianglesToRender[N_MESH_FACES];
vec3 cameraPosition = {
    .x = 0,
    .y = 0,
    .z = -5
};
vec3 cubeRotation     = {};
float fovFactor       = 640;
int previousFrameTime = 0;

bool isRunning = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    // Creating a SDL texture that is used to display the color buffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight
    );
}

void processInput(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            break;
    }
}

// simple naive orthographic projection
vec2 project(const vec3 point) {
    return (vec2){
        .x = point.x * fovFactor / point.z,
        .y = point.y * fovFactor / point.z,
    };
}


void update(void) {
    const int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    previousFrameTime = SDL_GetTicks();

    const float rotation = 0.005;
    cubeRotation.y += rotation;
    cubeRotation.z += rotation;
    cubeRotation.x += rotation;

    for (int i = 0; i < N_MESH_FACES; i++) {
        const face meshFace       = meshFaces[i];
        const vec3 faceVertices[] = {
            meshVertices[meshFace.a - 1],
            meshVertices[meshFace.b - 1],
            meshVertices[meshFace.c - 1],
        };

        triangle projectedTriangle;
        for (int j = 0; j < 3; j++) {
            vec3 transformedVertex = vec3_rotate_y(faceVertices[j], cubeRotation.y);
            transformedVertex      = vec3_rotate_x(transformedVertex, cubeRotation.x);
            transformedVertex      = vec3_rotate_z(transformedVertex, cubeRotation.z);

            // trnaslate vertex away from camera
            transformedVertex.z -= cameraPosition.z;
            vec2 projectedVertex = project(transformedVertex);

            // scale and translate the projected points to the middle of the screen
            projectedVertex.x += (windowWidth / 2);
            projectedVertex.y += (windowHeight / 2);
            projectedTriangle.points[j] = projectedVertex;
        }

        trianglesToRender[i] = projectedTriangle;
    }
}

void render(void) {
    drawGrid();

    drawLine(100, 300, 300, 30, 0xFF00FFFF);
    drawLine(300, 30, 400, 600, 0xFF00FFFF);
    drawLine(400, 600, 100, 300, 0xFF00FFFF);

    // render the projected triangles
    for (int i = 0; i < N_MESH_FACES; i++) {
        const triangle t = trianglesToRender[i];
        uint32_t color;
        if (i % 2 == 0) {
            color = 0xFF0000FF;
        } else {
            color = 0xFF00FF00;
        }
        // const uint32_t color = 0xFFFFFFFF;
        drawRect(t.points[0].x, t.points[0].y, 3, 3, 0xFFFFFFFF);
        drawRect(t.points[1].x, t.points[1].y, 3, 3, 0xFFFFFFFF);
        drawRect(t.points[2].x, t.points[2].y, 3, 3, 0xFFFFFFFF);
        // drawPixel(projectedPoint.x, projectedPoint.y, 0xFFFF00FF);
        drawTriangle(t.points[0], t.points[1], t.points[2], color);
    }

    renderColorBuffer();
    clearColorBuffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(void) {
    isRunning = initializeWindow();

    setup();

    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (isRunning) {
        processInput();
        update();
        render();
    }

    destroyWindow();

    return EXIT_SUCCESS;
}
