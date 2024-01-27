//
// Created by Hector Mejia on 1/23/24.
//

#include <stdint.h>
#include "light.h"

Light light = {
    .direction = {.x = 0, .y = 0, .z = 1}
};

///////////////////////////////////////////////////////////////////////////////
// Change color based on a percentage factor to represent light intensity
///////////////////////////////////////////////////////////////////////////////
uint32_t lightApplyIntensity(uint32_t originalColor, float percentageFactor) {
    if (percentageFactor < 0) percentageFactor = 0;
    if (percentageFactor > 1) percentageFactor = 1;

    uint32_t a = (originalColor & 0xFF000000);
    uint32_t r = (uint32_t)((float)(originalColor & 0x00FF0000) * percentageFactor);
    uint32_t g = (uint32_t)((float)(originalColor & 0x0000FF00) * percentageFactor);
    uint32_t b = (uint32_t)((float)(originalColor & 0x000000FF) * percentageFactor);

    uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    return new_color;
}
