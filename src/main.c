#include <stdbool.h>

#include <SDL2/SDL.h>

#include "display.h"

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

void update(void) {
    // TODO:
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawGrid();

    drawPixel(100, 100, 0xFFFF00FF);
    drawRect(300, 200, 300, 150, 0xFFFF00FF);

    renderColorBuffer();
    clearColorBuffer(0xFF000000);

    SDL_RenderPresent(renderer);
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

    return 0;
}
