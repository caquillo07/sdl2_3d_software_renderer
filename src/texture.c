//
// Created by Hector Mejia on 1/27/24.
//

#include "texture.h"
#include <stdio.h>
#include <stdint.h>
#include "upng.h"


upng_t *pngTexture = NULL;
uint32_t *meshTexture = NULL;
int textureWidth = 64;
int textureHeight = 64;

void loadPNGTextureData(const char *fileName) {
    pngTexture = upng_new_from_file(fileName);
    if (pngTexture == NULL) {
        printf("Error loading texture: %s\n", (upng_get_error(pngTexture)));
        return;
    }

    upng_decode(pngTexture);
    if (upng_get_error(pngTexture) != UPNG_EOK) {
        printf("Error loading texture: %s\n", upng_get_error(pngTexture));
        return;
    }
    textureWidth = upng_get_width(pngTexture);
    textureHeight = upng_get_height(pngTexture);
    meshTexture = (uint32_t *) upng_get_buffer(pngTexture);
}
