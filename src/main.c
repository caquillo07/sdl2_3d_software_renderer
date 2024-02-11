#include <stdbool.h>
#include <SDL2/SDL.h>

#include "array.h"
#include "display.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "vector.h"
#include "texture.h"

#define nil NULL;

Triangle *trianglesToRender = nil;
Vec3 cameraPosition = {
    .x = 0,
    .y = 0,
    .z = 0,
};
Mat4 projectionMatrix;

Uint32 previousFrameTime = 0;

bool isRunning = false;
bool isPaused = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    renderMethod = RENDER_WIRE;
    cullMethod = CULL_BACKFACE;
    colorBuffer = (uint32_t *) malloc(sizeof(uint32_t) * windowWidth * windowHeight);

    // // Creating a SDL texture that is used to display the color buffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight
    );

    // init perspective projection matrix
    const float fov = M_PI / 3.0f;  // the same as 180/3, or 60 degrees
    projectionMatrix = mat4_makePerspective(
        fov,
        (float) windowHeight / (float) windowWidth,
        0.1f,
        100.0f
    );

    // manually load the hardcoded texture data from the static array
    meshTexture = (uint32_t *) REDBRICK_TEXTURE;
    textureWidth = 64;
    textureHeight = 64;

    // loadOBJFileData("../assets/cube.obj");
    loadOBJFileData("../assets/f22.obj");
//    loadCubeMeshData();
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
            if (event.key.keysym.sym == SDLK_1) {
                renderMethod = RENDER_WIRE_VERTEX;
            }
            if (event.key.keysym.sym == SDLK_2) {
                renderMethod = RENDER_WIRE;
            }
            if (event.key.keysym.sym == SDLK_3) {
                renderMethod = RENDER_FILL_TRIANGLE;
            }
            if (event.key.keysym.sym == SDLK_4) {
                renderMethod = RENDER_FILL_TRIANGLE_WIRE;
            }
            if (event.key.keysym.sym == SDLK_5) {
                renderMethod = RENDER_TEXTURED;
            }
            if (event.key.keysym.sym == SDLK_6) {
                renderMethod = RENDER_TEXTURED_WIRE;
            }
            if (event.key.keysym.sym == SDLK_c) {
                cullMethod = CULL_BACKFACE;
            }
            if (event.key.keysym.sym == SDLK_d) {
                cullMethod = CULL_NONE;
            }
            if (event.key.keysym.sym == SDLK_SPACE) {
                isPaused = !isPaused;
            }
            break;
        default:
            break;
    }
}

void update(void) {
    const int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    previousFrameTime = SDL_GetTicks();

    // each update init the triangles to render, todo(hector) - dont do this
    trianglesToRender = nil;

    if (!isPaused) {
        const float rotation = 0.005f;
        mesh.rotation.x += rotation;
        mesh.rotation.y += rotation;
        mesh.rotation.z += rotation;
//        mesh.scale.x += 0.002f;
//        mesh.scale.y += 0.002f;
//        mesh.scale.z += 0.002f;
//        mesh.translation.x += 0.008f;
//        mesh.translation.y += 0.002f;
        mesh.translation.z = 5.f;
    }

    for (int i = 0; i < array_length(mesh.faces); i++) {
        const Face meshFace = mesh.faces[i];
        const Vec3 faceVertices[] = {
            mesh.vertices[meshFace.a - 1],
            mesh.vertices[meshFace.b - 1],
            mesh.vertices[meshFace.c - 1],
        };

        Vec4 transformedVertices[3];
        for (int j = 0; j < 3; j++) {
            Vec4 transformedVertex = vec4_fromVec3(faceVertices[j]);
            Mat4 worldMatrix = mat4_makeWorld(mesh.translation, mesh.rotation, mesh.scale);
            transformedVertex = mat4_mulVec4(worldMatrix, transformedVertex);

            transformedVertices[j] = transformedVertex;
        }

        // triangle culling
        /*   A
         *  /  \
         * B----C */
        const Vec3 vectorA = vec3_fromVec4(transformedVertices[0]);
        const Vec3 vectorB = vec3_fromVec4(transformedVertices[1]);
        const Vec3 vectorC = vec3_fromVec4(transformedVertices[2]);
        Vec3 vectorAB = vec3_sub(vectorB, vectorA);
        Vec3 vectorAC = vec3_sub(vectorC, vectorA);
        vec3_normalize(&vectorAB);
        vec3_normalize(&vectorAC);

        // compute face normal using the cross product to find the perpendicular.
        // the order matters, we are using a left-handed coordinate system (Z
        // grows inside the screen).
        Vec3 normal = vec3_cross(vectorAB, vectorAC);

        // normalize the normal
        vec3_normalize(&normal);

        // find the vector between a point in the triangle and the camera origin
        const Vec3 cameraRay = vec3_sub(cameraPosition, vectorA);

        if (cullMethod == CULL_BACKFACE) {
            // check if this triangle is aligned with the screen
            // bypass the triangles that are looking away from the camera
            if (vec3_dot(normal, cameraRay) < 0) {
                continue;;
            }
        }

        // loop all three vertices to perform projection
        Vec4 projectedPoints[3];
        for (int j = 0; j < 3; j++) {
            projectedPoints[j] = mat4_mulVec4Project(projectionMatrix, transformedVertices[j]);
            // Scale into the viewport (has to go first)
            projectedPoints[j].x *= (float) windowWidth / 2.0f;
            projectedPoints[j].y *= (float) windowHeight / 2.0f;

            // in screen space, invert Y values to account for flipped screen coordinates
            projectedPoints[j].y *= -1;

            // translate the projected points to the middle of the screen
            projectedPoints[j].x += (float) windowWidth / 2.0f;
            projectedPoints[j].y += (float) windowHeight / 2.0f;
        }

        // calculate the average depth of each face based on the vertices after
        // the transformations
        float avgDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3;

        // Calculate the shade of the triangle based on the direction of the light
        // and the normal of the face.
        // we need the inverse of the normal to calculate the light intensity because
        // our Z grows towards the screen, not from the screen.
        float lightIntensityFactor = -1 * vec3_dot(normal, light.direction);

        uint32_t triangleColor = lightApplyIntensity(meshFace.color, lightIntensityFactor);

        Triangle projectedTriangle = {
            .points = {
                {projectedPoints[0].x, projectedPoints[0].y},
                {projectedPoints[1].x, projectedPoints[1].y},
                {projectedPoints[2].x, projectedPoints[2].y},
            },
            .textCoords = {
                {meshFace.vertexA_UV.u, meshFace.vertexA_UV.v},
                {meshFace.vertexB_UV.u, meshFace.vertexB_UV.v},
                {meshFace.vertexC_UV.u, meshFace.vertexC_UV.v},
            },
            .color = triangleColor,
            .avgDepth = avgDepth,
        };

        array_push(trianglesToRender, projectedTriangle);
    }

    // bubble sort the triangles to render their avgDepth.
    // slow but meh, this is a demo any ways
    int numTriangles = array_length(trianglesToRender);
    for (int i = 0; i < numTriangles; ++i) {
        for (int j = i; j < numTriangles; ++j) {
            if (trianglesToRender[i].avgDepth < trianglesToRender[j].avgDepth) {
                Triangle tmp = trianglesToRender[i];
                trianglesToRender[i] = trianglesToRender[j];
                trianglesToRender[j] = tmp;
            }
        }
    }
}

void render(void) {
    drawGrid();

    // Loop all projected triangles and render them
    // render the projected triangles
    const int numOfTriangles = array_length(trianglesToRender);
    for (int i = 0; i < numOfTriangles; i++) {
        const Triangle triangle = trianglesToRender[i];

        if (renderMethod == RENDER_FILL_TRIANGLE || renderMethod == RENDER_FILL_TRIANGLE_WIRE) {
            drawFilledTriangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                triangle.color
            );
        }

        if (renderMethod == RENDER_TEXTURED || renderMethod == RENDER_TEXTURED_WIRE) {
            drawTexturedTriangle(
                triangle.points[0].x, triangle.points[0].y, triangle.textCoords[0].u, triangle.textCoords[0].v, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.textCoords[1].u, triangle.textCoords[1].v, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.textCoords[2].u, triangle.textCoords[2].v, // vertex C
                meshTexture
            );
        }

        if (renderMethod == RENDER_WIRE ||
            renderMethod == RENDER_WIRE_VERTEX ||
            renderMethod == RENDER_FILL_TRIANGLE_WIRE ||
            renderMethod == RENDER_TEXTURED_WIRE) {
            drawTriangle(
                triangle.points[0],
                triangle.points[1],
                triangle.points[2],
                0xFFFFFFF
            );
        }

        if (renderMethod == RENDER_WIRE_VERTEX) {
            const uint32_t dotColor = 0xFFFF0000;
            drawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, dotColor);
            drawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, dotColor);
            drawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, dotColor);
        }
    }

    drawFilledTriangle(300, 300, 200, 500, 500, 700, 0xFF00FF00);

    // clear triangles - todo - dont do this
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

    while (isRunning) {
        processInput();
        update();
        render();
    }

    destroyWindow();
    freeResources();

    return EXIT_SUCCESS;
}
