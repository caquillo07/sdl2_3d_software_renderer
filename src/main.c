#include <stdbool.h>
#include <SDL2/SDL.h>

#include "array.h"
#include "display.h"
#include "mesh.h"
#include "vector.h"

#define nil NULL;

Triangle *trianglesToRender = nil;
Vec3 cameraPosition = {
    .x = 0,
    .y = 0,
    .z = 0,
};
float fovFactor = 640;
int previousFrameTime = 0;

bool isRunning = false;
bool isWireframeMode = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    colorBuffer = (uint32_t *) malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    // // Creating a SDL texture that is used to display the color buffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight
    );

    // loadOBJFileData("../assets/cube.obj");
    loadOBJFileData("../assets/f22.obj");
}

void processInput(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            if (event.key.keysym.sym == SDLK_F1) {
                isWireframeMode = !isWireframeMode;
            }
            break;
        default:break;
    }
}

// simple naive perspective projection
Vec2 project(const Vec3 point) {
    return (Vec2) {
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
    mesh.rotation.x += rotation;
    mesh.rotation.y += rotation;
    mesh.rotation.z += rotation;

    for (int i = 0; i < array_length(mesh.faces); i++) {
        const Face meshFace = mesh.faces[i];
        const Vec3 faceVertices[] = {
            mesh.vertices[meshFace.a - 1],
            mesh.vertices[meshFace.b - 1],
            mesh.vertices[meshFace.c - 1],
        };

        Vec3 transformedVertices[3];
        for (int j = 0; j < 3; j++) {
            Vec3 transformedVertex = vec3_rotateY(faceVertices[j], mesh.rotation.y);
            transformedVertex = vec3_rotateX(transformedVertex, mesh.rotation.x);
            transformedVertex = vec3_rotateY(transformedVertex, mesh.rotation.z);

            // trnaslate vertex away from camera
            transformedVertex.z += 5; // pushing everything inside the screen 5 units
            transformedVertices[j] = transformedVertex;
        }

        // triangle culling
        /*   A
         *  /  \
         * B----C */
        const Vec3 vectorA = transformedVertices[0];
        const Vec3 vectorB = transformedVertices[1];
        const Vec3 vectorC = transformedVertices[2];

        const Vec3 vectorAB = vec3_sub(vectorB, vectorA);
        const Vec3 vectorAC = vec3_sub(vectorC, vectorA);
        // vec3_normalize(&vectorAB);
        // vec3_normalize(&vectorAC);

        // compute face normal using the cross product to find the perpendicular.
        // the order matters, we are using a left handed coordinate system (Z
        // grows inside the screen).
        Vec3 normal = vec3_cross(vectorAB, vectorAC);

        // normalize the normal
        vec3_normalize(&normal);

        // find the vector between a point in the triangle and the camera origin
        const Vec3 cameraRay = vec3_sub(cameraPosition, vectorA);

        // check if this triangle is aligned with the screen
        // bypass the triangles that are looking away from the camera
        if (vec3_dot(normal, cameraRay) < 0) {
            continue;;
        }

        // loop all three vertices to perform projection
        Triangle projectedTriangle;
        for (int j = 0; j < 3; j++) {
            Vec2 projectedVertex = project(transformedVertices[j]);

            // scale and translate the projected points to the middle of the screen
            projectedVertex.x += (windowWidth / 2);
            projectedVertex.y += (windowHeight / 2);
            projectedTriangle.points[j] = projectedVertex;
        }

        array_push(trianglesToRender, projectedTriangle);
    }
}

void render(void) {
    drawGrid();

    // Loop all projected triangles and render them


    // render the projected triangles
    const int numOfTriangles = array_length(trianglesToRender);
    for (int i = 0; i < numOfTriangles; i++) {
        const Triangle t = trianglesToRender[i];
        const uint32_t color = 0xFF00FF00;

        // drawing the actual triangle
        if (isWireframeMode) {
            drawTriangle(t.points[0], t.points[1], t.points[2], color);
        } else {
            drawFilledTriangle(
                t.points[0].x, t.points[0].y,
                t.points[1].x, t.points[1].y,
                t.points[2].x, t.points[2].y,
                color
            );

            drawTriangle(
                t.points[0],
                t.points[1],
                t.points[2],
                0xFF00000
            );
        }
    }

    drawFilledTriangle(300, 300, 200, 500, 500, 700, 0xFF00FF00);

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
