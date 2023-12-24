#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* colorBuffer;
extern SDL_Texture* colorBufferTexture;
extern int windowWidth;
extern int windowHeight;

bool initializeWindow(void);
void drawGrid(void);
void drawRect(int x, int y, int width, int height, uint32_t color);
void drawPixel(const int x, const int y, const uint32_t color);
void renderColorBuffer(void);
void clearColorBuffer(uint32_t color);
void destroyWindow(void);

#endif
