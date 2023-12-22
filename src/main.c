#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

bool isRunning         = false;
SDL_Window* window     = NULL;
SDL_Renderer* renderer = NULL;

bool initializeWindow(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
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

void setup(void) {
    // TODO:
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
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    //...

    SDL_RenderPresent(renderer);
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

    return 0;
}
