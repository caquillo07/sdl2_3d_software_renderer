//
// Created by Hector Mejia on 1/23/24.
//

#ifndef SDL2_SOFTWARE_RENDERER_LIGHT_H
#define SDL2_SOFTWARE_RENDERER_LIGHT_H

#include <stdint.h>
#include "vector.h"

typedef struct {
    Vec3 direction;
} Light;

extern Light light;

uint32_t lightApplyIntensity(uint32_t originalColor, float percentageFactor);

#endif //SDL2_SOFTWARE_RENDERER_LIGHT_H
