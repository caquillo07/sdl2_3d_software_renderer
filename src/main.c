#include <stdbool.h>
#include <SDL2/SDL.h>

#include "upng.h"
#include "array.h"
#include "display.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "vector.h"
#include "texture.h"
#include "camera.h"
#include "clipping.h"

#define MAX_TRIANGLES 10000
Triangle trianglesToRender[MAX_TRIANGLES];
int numTrianglesToRender = 0;
float deltaTime = 0.0f;

Mat4 projectionMatrix;

Uint32 previousFrameTime = 0;

bool isRunning = false;
bool isPaused = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    renderMethod = RENDER_TEXTURED;
    cullMethod = CULL_BACKFACE;
    colorBuffer = (uint32_t *) malloc(sizeof(uint32_t) * windowWidth * windowHeight);
    zBuffer = (float *) malloc(sizeof(float) * windowWidth * windowHeight);

    // // Creating a SDL texture that is used to display the color buffer
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight
    );

    // capture the mouse
//    SDL_SetRelativeMouseMode(SDL_TRUE);

    // init perspective projection matrix
    const float aspectX = (float) windowWidth / (float) windowHeight;
    const float aspectY = (float) windowHeight / (float) windowWidth;
    const float fovY = M_PI / 3.0f;  // the same as 180/3, or 60 degrees
    const float fovX = 2.0f * atanf(tanf(fovY / 2.0f) * aspectX);
    const float zNear = 1.f;
    const float zFar = 20.0f;
    projectionMatrix = mat4_makePerspective(fovY, aspectY, zNear, zFar);

    // init the frustum planes
    initFrustumPlanes(fovX, fovY, zNear, zFar);

    loadOBJFileData("../assets/cube.obj");
    loadPNGTextureData("../assets/cube.png");
}

void processInput(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEMOTION:
            break;
            printf("Mouse moved to (%d, %d) - (%d, %d)\n", event.motion.x, event.motion.y, event.motion.xrel,
                   event.motion.yrel);
            if (event.motion.xrel != 0) {
                camera.yawAngle += event.motion.xrel * 0.05f * deltaTime;
            }
            if (event.motion.yrel != 0) {
                camera.pitchAngle += event.motion.yrel * 0.05f * deltaTime;
            }
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
            if (event.key.keysym.sym == SDLK_x) {
                cullMethod = CULL_NONE;
            }
            if (event.key.keysym.sym == SDLK_SPACE) {
                isPaused = !isPaused;
            }
            if (event.key.keysym.sym == SDLK_UP) {
                camera.position.y += 3.0f * deltaTime;
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                camera.position.y -= 3.0f * deltaTime;
            }

            if (event.key.keysym.sym == SDLK_w) {
                camera.forwardVelocity = vec3_mul(camera.direction, 5.0f * deltaTime);
                camera.position = vec3_add(camera.position, camera.forwardVelocity);
            }
            if (event.key.keysym.sym == SDLK_s) {
                camera.forwardVelocity = vec3_mul(camera.direction, 5.0f * deltaTime);
                camera.position = vec3_sub(camera.position, camera.forwardVelocity);
            }
            if (event.key.keysym.sym == SDLK_a) {
                camera.yawAngle -= 1.0f * deltaTime;
            }
            if (event.key.keysym.sym == SDLK_d) {
                camera.yawAngle += 1.0f * deltaTime;
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
    // get the delta time in seconds
    deltaTime = (SDL_GetTicks() - previousFrameTime) / 1000.0f;

    // calculate the fps
//    const float fps = 1.0f / deltaTime;
//    printf("FPS: %f\n", fps);

    previousFrameTime = SDL_GetTicks();

    // reset the number of triangles to render for the current frame
    numTrianglesToRender = 0;

    if (!isPaused) {
        const float rotation = 0.5f;
//        mesh.rotation.x += rotation * deltaTime;
//        mesh.rotation.y += rotation * deltaTime;
//        mesh.rotation.z += rotation * deltaTime;
//        mesh.scale.x += 0.002f;
//        mesh.scale.y += 0.002f;
//        mesh.scale.z += 0.002f;
//        mesh.translation.x += 1 * deltaTime;
//        mesh.translation.y += 0.002f;
        mesh.translation.z = 5.0f;
    }

    // create the view matrix
    Vec3 upDuration = {0, 1, 0};
    Vec3 target = {0, 0, 1};
    Mat4 cameraYawRotation = mat4_makeRotationY(camera.yawAngle);
    Mat4 cameraPitchRotation = mat4_makeRotationX(camera.pitchAngle);
    camera.direction = vec3_fromVec4(mat4_mulVec4(cameraPitchRotation, vec4_fromVec3(target)));
    camera.direction = vec3_fromVec4(mat4_mulVec4(cameraYawRotation, vec4_fromVec3(camera.direction)));

    // offset the camera position in the direction where the camera is pointing at
    target = vec3_add(camera.position, camera.direction);

    Mat4 viewMatrix = mat4_lookAt(camera.position, target, upDuration);

    for (int i = 0; i < array_length(mesh.faces); i++) {
        const Face meshFace = mesh.faces[i];
        const Vec3 faceVertices[] = {
            mesh.vertices[meshFace.a],
            mesh.vertices[meshFace.b],
            mesh.vertices[meshFace.c],
        };

        Vec4 transformedVertices[3];
        for (int j = 0; j < 3; j++) {
            Vec4 transformedVertex = vec4_fromVec3(faceVertices[j]);
            Mat4 worldMatrix = mat4_makeWorld(mesh.translation, mesh.rotation, mesh.scale);
            transformedVertex = mat4_mulVec4(worldMatrix, transformedVertex);

            // transform the vertex using the view matrix
            transformedVertex = mat4_mulVec4(viewMatrix, transformedVertex);

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

        Vec3 origin = {0, 0, 0};
        // find the vector between a point in the triangle and the camera origin
        const Vec3 cameraRay = vec3_sub(origin, vectorA);

        if (cullMethod == CULL_BACKFACE) {
            // check if this triangle is aligned with the screen
            // bypass the triangles that are looking away from the camera
            if (vec3_dot(normal, cameraRay) < 0) {
                continue;;
            }
        }

        // Clipping
        // clip the triangle against the near plane
        Polygon polygon = createPolygonFromTriangle(
            vec3_fromVec4(transformedVertices[0]),
            vec3_fromVec4(transformedVertices[1]),
            vec3_fromVec4(transformedVertices[2]),
            meshFace.vertexA_UV,
            meshFace.vertexB_UV,
            meshFace.vertexC_UV
        );

        clipPolygon(&polygon);

        // break the polygon into triangles
        Triangle trianglesAfterClipping[MAX_NUM_POLY_TRIANGLES];
        int numTrianglesAfterClipping = 0;

        // trianglesAfterClipping gets passed by reference here in C
        createTrianglesFromPolygon(&polygon, trianglesAfterClipping, &numTrianglesAfterClipping);

        // loop all the triangles after clipping
        for (int t = 0; t < numTrianglesAfterClipping; t++) {
            Triangle clippedTriangle = trianglesAfterClipping[t];

            // loop all three vertices to perform projection
            Vec4 projectedPoints[3];
            for (int j = 0; j < 3; j++) {
                projectedPoints[j] = mat4_mulVec4Project(projectionMatrix, clippedTriangle.points[j]);


                // in screen space, invert Y values to account for flipped screen coordinates
                projectedPoints[j].y *= -1;

                // Scale into the viewport (has to go first)
                projectedPoints[j].x *= (float) windowWidth / 2.0f;
                projectedPoints[j].y *= (float) windowHeight / 2.0f;

                // translate the projected points to the middle of the screen
                projectedPoints[j].x += (float) windowWidth / 2.0f;
                projectedPoints[j].y += (float) windowHeight / 2.0f;
            }


            // Calculate the shade of the triangle based on the direction of the light
            // and the normal of the face.
            // we need the inverse of the normal to calculate the light intensity because
            // our Z grows towards the screen, not from the screen.
            float lightIntensityFactor = -1 * vec3_dot(normal, light.direction);

            uint32_t triangleColor = lightApplyIntensity(meshFace.color, lightIntensityFactor);

            Triangle triangleToRender = {
                .points = {
                    {projectedPoints[0].x, projectedPoints[0].y, projectedPoints[0].z, projectedPoints[0].w},
                    {projectedPoints[1].x, projectedPoints[1].y, projectedPoints[1].z, projectedPoints[1].w},
                    {projectedPoints[2].x, projectedPoints[2].y, projectedPoints[2].z, projectedPoints[2].w},
                },
                .textCoords = {
                    {clippedTriangle.textCoords[0].u, clippedTriangle.textCoords[0].v},
                    {clippedTriangle.textCoords[1].u, clippedTriangle.textCoords[1].v},
                    {clippedTriangle.textCoords[2].u, clippedTriangle.textCoords[2].v},
                },
                .color = triangleColor,
            };
            if (numTrianglesToRender < MAX_TRIANGLES) {
                trianglesToRender[numTrianglesToRender] = triangleToRender;
                numTrianglesToRender++;
            }
        }
    }
}

void render(void) {
    drawGrid();

    // Loop all projected triangles and render them
    // render the projected triangles
    for (int i = 0; i < numTrianglesToRender; i++) {
        const Triangle triangle = trianglesToRender[i];

        if (renderMethod == RENDER_FILL_TRIANGLE || renderMethod == RENDER_FILL_TRIANGLE_WIRE) {
            drawFilledTriangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, // vertex C
                triangle.color
            );
        }

        if (renderMethod == RENDER_TEXTURED || renderMethod == RENDER_TEXTURED_WIRE) {
            drawTexturedTriangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
                triangle.textCoords[0].u, triangle.textCoords[0].v, // vertex A
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
                triangle.textCoords[1].u, triangle.textCoords[1].v, // vertex B
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
                triangle.textCoords[2].u, triangle.textCoords[2].v, // vertex C
                meshTexture
            );
        }

        if (renderMethod == RENDER_WIRE ||
            renderMethod == RENDER_WIRE_VERTEX ||
            renderMethod == RENDER_FILL_TRIANGLE_WIRE ||
            renderMethod == RENDER_TEXTURED_WIRE) {

            Vec2 a = vec2_fromVec4(triangle.points[0]);
            Vec2 b = vec2_fromVec4(triangle.points[1]);
            Vec2 c = vec2_fromVec4(triangle.points[2]);
            drawTriangle(a, b, c, 0xFFFFFFF);
        }

        if (renderMethod == RENDER_WIRE_VERTEX) {
            const uint32_t dotColor = 0xFFFF0000;
            drawRect(
                (int) triangle.points[0].x - 3,
                (int) triangle.points[0].y - 3,
                6,
                6,
                dotColor
            );
            drawRect(
                (int) triangle.points[1].x - 3,
                (int) triangle.points[1].y - 3,
                6,
                6,
                dotColor
            );
            drawRect(
                (int) triangle.points[2].x - 3,
                (int) triangle.points[2].y - 3,
                6,
                6,
                dotColor
            );
        }
    }

    renderColorBuffer();
    clearColorBuffer(0xFF000000);
    clearZBuffer();

    SDL_RenderPresent(renderer);
}

void freeResources(void) {
    freeMesh();
    free(colorBuffer);
    free(zBuffer);
    upng_free(pngTexture);
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
