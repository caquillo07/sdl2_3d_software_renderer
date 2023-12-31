#include <stdbool.h>
#include <SDL2/SDL.h>

#include "array.h"
#include "display.h"
#include "mesh.h"
#include "vector.h"

#define nil NULL;

Triangle* trianglesToRender = nil;
Vec3 cameraPosition         = {
    .x = 0,
    .y = 0,
    .z = -5
};
float fovFactor       = 640;
int previousFrameTime = 0;

bool isRunning = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    // // Creating a SDL texture that is used to display the color buffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight
    );

    // loadCubeMeshData();
    loadOBJFileData("../assets/f22.obj");
}

void processInput(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
        default:
            break;
    }
}

// simple naive orthographic projection
Vec2 project(const Vec3 point) {
    return (Vec2){
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

    // each update init the triangles to render, todo(hector) - dont do this
    trianglesToRender = nil;

    const float rotation = 0.005;
    mesh.rotation.y += rotation;
    mesh.rotation.z += rotation;
    mesh.rotation.x += rotation;

    for (int i = 0; i < array_length(mesh.faces); i++) {
        const Face meshFace       = mesh.faces[i];
        const Vec3 faceVertices[] = {
            mesh.vertices[meshFace.a - 1],
            mesh.vertices[meshFace.b - 1],
            mesh.vertices[meshFace.c - 1],
        };

        Triangle projectedTriangle;
        for (int j = 0; j < 3; j++) {
            Vec3 transformedVertex = vec3_rotate_y(faceVertices[j], mesh.rotation.y);
            transformedVertex      = vec3_rotate_x(transformedVertex, mesh.rotation.x);
            transformedVertex      = vec3_rotate_z(transformedVertex, mesh.rotation.z);

            // trnaslate vertex away from camera
            transformedVertex.z -= cameraPosition.z;
            Vec2 projectedVertex = project(transformedVertex);

            // scale and translate the projected points to the middle of the screen
            projectedVertex.x += (windowWidth / 2);
            projectedVertex.y += (windowHeight / 2);
            projectedTriangle.points[j] = projectedVertex;
        }

        // trianglesToRender[i] = projectedTriangle;
        array_push(trianglesToRender, projectedTriangle);
    }
}

void render(void) {
    drawGrid();

    drawLine(100, 300, 300, 30, 0xFF00FFFF);
    drawLine(300, 30, 400, 600, 0xFF00FFFF);
    drawLine(400, 600, 100, 300, 0xFF00FFFF);

    // render the projected triangles
    const int numOfTriangles = array_length(trianglesToRender);
    for (int i = 0; i < numOfTriangles; i++) {
        const Triangle t = trianglesToRender[i];
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

    // clear triangles - todo dont do this
    array_free(trianglesToRender);
    renderColorBuffer();
    clearColorBuffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void freeResources(void) {
    freeMesh();
    free(colorBuffer);
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
    freeResources();

    return EXIT_SUCCESS;
}
