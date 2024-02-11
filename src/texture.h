//
// Created by Hector Mejia on 1/27/24.
//

#ifndef SDL2_SOFTWARE_RENDERER_TEXTURE_H
#define SDL2_SOFTWARE_RENDERER_TEXTURE_H

#include <stdint.h>

typedef struct {
    float u;
    float v;
} Texture2;

extern int textureWidth;
extern int textureHeight;
extern uint32_t* meshTexture;

extern const uint8_t REDBRICK_TEXTURE[];

#endif //SDL2_SOFTWARE_RENDERER_TEXTURE_H
