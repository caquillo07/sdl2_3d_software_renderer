#include "display.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static uint32_t *colorBuffer = NULL;
static float *zBuffer = NULL;

static SDL_Texture *colorBufferTexture = NULL;
// if you want to downscale the image for a pixelated look
//static int windowWidth = 320;
//static int windowHeight = 200;

static int windowWidth = 800;
static int windowHeight = 600;

static enum CullMethod cullMethod = CULL_BACKFACE;
static enum RenderMethod renderMethod = RENDER_TEXTURED;

bool startFullScreen = false;

bool initializeWindow(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Set width and height of the SDL window with the max screen resolution
    int newWindowWidth = windowWidth;
    int newWindowHeight = windowHeight;
    if (startFullScreen) {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        newWindowWidth = displayMode.w;
        newWindowHeight = displayMode.h;
    }
    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        newWindowWidth,
        newWindowHeight,
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

    // Allocate the required memory in bytes to hold the color buffer
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
    if (x < 0 || x >= windowWidth || y >= windowHeight || y < 0) {
        return;
    }

    if (colorBuffer == NULL) {
        fprintf(stderr, "colorBuffer is not initialized.\n");
        return;
    }
    colorBuffer[(windowWidth * y) + x] = color;
}

void drawLine(const int x0, const int y0, const int x1, const int y1, const uint32_t color) {
    const int deltaX = x1 - x0;
    const int deltaY = y1 - y0;

    const int sideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);
    const float xIncrement = deltaX / (float) sideLength;
    const float yIncrement = deltaY / (float) sideLength;

    float currentX = x0;
    float currentY = y0;

    for (int i = 0; i <= sideLength; i++) {
        drawPixel(round(currentX), round(currentY), color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}

void drawLinePoint(const Vec2 point0, const Vec2 point1, const uint32_t color) {
    drawLine(point0.x, point0.y, point1.x, point1.y, color);
}

void drawTriangle(const Vec2 pointA, const Vec2 pointB, const Vec2 pointC, const uint32_t color) {
    drawLinePoint(pointA, pointB, color);
    drawLinePoint(pointB, pointC, color);
    drawLinePoint(pointC, pointA, color);
}

void renderColorBuffer(void) {
    SDL_UpdateTexture(
        colorBufferTexture,
        NULL,
        colorBuffer,
        windowWidth * sizeof(uint32_t)
    );
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void clearColorBuffer(const uint32_t color) {
    if (colorBuffer == NULL) {
        return;
    }
    for (int i = 0; i < windowHeight * windowWidth; i++) {
        colorBuffer[i] = color;
    }
}

void clearZBuffer(void) {
    if (zBuffer == NULL) {
        return;
    }
    for (int i = 0; i < windowHeight * windowWidth; i++) {
        // we have a left-handed coordinate system, so the zBuffer is filled with 1s
        zBuffer[i] = 1.f; // 1 is the farthest point in the zBuffer
    }
}

void destroyWindow(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(colorBuffer);
    free(zBuffer);
    SDL_DestroyTexture(colorBufferTexture);
}

int getWindowHeight(void) {
    return windowHeight;
}

int getWindowWidth(void) {
    return windowWidth;
}

void setCullMethod(enum CullMethod method) {
    cullMethod = method;
}

void setRenderMethod(enum RenderMethod method) {
    renderMethod = method;
}

enum RenderMethod getRenderMethod(void) {
    return renderMethod;
}

enum CullMethod getCullMethod(void) {
    return cullMethod;
}

bool shouldRenderFilledTriangle(void) {
    return renderMethod == RENDER_FILL_TRIANGLE ||
           renderMethod == RENDER_FILL_TRIANGLE_WIRE;
}

bool shouldRenderTexturedTriangle(void) {
    return renderMethod == RENDER_TEXTURED ||
           renderMethod == RENDER_TEXTURED_WIRE;
}

bool shouldRenderWireframe(void) {
    return renderMethod == RENDER_WIRE ||
           renderMethod == RENDER_WIRE_VERTEX ||
           renderMethod == RENDER_FILL_TRIANGLE_WIRE ||
           renderMethod == RENDER_TEXTURED_WIRE;
}

bool shouldRenderWireVertex(void) {
    return renderMethod == RENDER_WIRE_VERTEX;
}

float getZBufferAt(int x, int y) {
    if (x < 0 || x >= windowWidth || y >= windowHeight || y < 0) {
        return 1.f;
    }
    return zBuffer[(windowWidth * y) + x];
}

void updateZBuffer(int x, int y, float value) {
    if (x < 0 || x >= windowWidth || y >= windowHeight || y < 0) {
        return;
    }
    zBuffer[(windowWidth * y) + x] = value;
}
