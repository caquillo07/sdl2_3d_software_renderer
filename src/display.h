#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "vector.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* colorBuffer;
extern SDL_Texture* colorBufferTexture;
extern int windowWidth;
extern int windowHeight;

bool initializeWindow(void);
void drawGrid(void);
void drawRect(int x, int y, int width, int height, uint32_t color);
void drawPixel(int x, int y, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color);
void drawLinePoint(Vec2 point0, Vec2 point1, uint32_t color);
void drawTriangle(Vec2 pointA, Vec2 pointB, Vec2 pointC, uint32_t color);
void renderColorBuffer(void);
void clearColorBuffer(uint32_t color);
void destroyWindow(void);

#endif
