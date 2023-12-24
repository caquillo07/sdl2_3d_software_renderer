#include "display.h"

SDL_Window* window              = NULL;
SDL_Renderer* renderer          = NULL;
uint32_t* colorBuffer           = NULL;
SDL_Texture* colorBufferTexture = NULL;
int windowWidth                 = 800;
int windowHeight                = 600;

bool initializeWindow(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Set width and height of the SDL window with the max screen resolution
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth  = displayMode.w;
    windowHeight = displayMode.h;

    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    // Create a SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

void drawGrid(void) {
    for (int y = 0; y < windowHeight; y += 10) {
        for (int x = 0; x < windowWidth; x += 10) {
            colorBuffer[(windowWidth * y) + x] = 0xFF444444;
        }
    }
}

void drawRect(const int x, const int y, const int width, const int height, const uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            const int currentX = x + i;
            const int currentY = y + j;

            drawPixel(currentX, currentY, color);
        }
    }
}

void drawPixel(const int x, const int y, const uint32_t color) {
    colorBuffer[(windowWidth * y) + x] = color;
}

void renderColorBuffer(void) {
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        windowWidth * sizeof(uint32_t)
    );
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void clearColorBuffer(const uint32_t color) {
    for (int y = 0; y < windowHeight; y++) {
        for (int x = 0; x < windowWidth; x++) {
            colorBuffer[(windowWidth * y) + x] = color;
        }
    }
}

void destroyWindow(void) {
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
