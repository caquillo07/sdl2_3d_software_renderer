#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"

#define FPS 120
#define FRAME_TARGET_TIME (1000 / FPS)

enum CullMethod {
    CULL_NONE,
    CULL_BACKFACE,
};

enum RenderMethod {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE,
};

int getWindowWidth(void);

int getWindowHeight(void);

void setRenderMethod(enum RenderMethod method);

enum RenderMethod getRenderMethod(void);

void setCullMethod(enum CullMethod method);

enum CullMethod getCullMethod(void);

bool shouldRenderFilledTriangle(void);
bool shouldRenderTexturedTriangle(void);
bool shouldRenderWireframe(void);
bool shouldRenderWireVertex(void);
bool initializeWindow(void);

void drawGrid(void);
void drawRect(int x, int y, int width, int height, uint32_t color);
void drawPixel(int x, int y, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
void drawLinePoint(Vec2 point0, Vec2 point1, uint32_t color);
void drawTriangle(Vec2 pointA, Vec2 pointB, Vec2 pointC, uint32_t color);

void renderColorBuffer(void);
void clearColorBuffer(uint32_t color);
void clearZBuffer(void);
void destroyWindow(void);

float getZBufferAt(int x, int y);
void updateZBuffer(int x, int y, float value);

#endif
