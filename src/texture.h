//
// Created by Hector Mejia on 1/27/24.
//

#ifndef SDL2_SOFTWARE_RENDERER_TEXTURE_H
#define SDL2_SOFTWARE_RENDERER_TEXTURE_H

#include <stdint.h>
#include "upng.h"

typedef struct {
    float u;
    float v;
} Texture2;

extern int textureWidth;
extern int textureHeight;

extern upng_t* pngTexture;
extern uint32_t* meshTexture;

extern const uint8_t REDBRICK_TEXTURE[];

void loadPNGTextureData(const char *fileName);

Texture2 texture2_clone(Texture2 *t);

#endif //SDL2_SOFTWARE_RENDERER_TEXTURE_H
