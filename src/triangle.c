//
// Created by Hector Mejia on 12/26/23.
//

#include "triangle.h"

#include "display.h"

void intSwap(int* a, int* b) {
    const int tmp = *a;
    *a            = *b;
    *b            = tmp;
}

// todo(hector) - get cool comment that shows the triangle we are drawing here
void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // find the two slopes from the two triangle legs
    // because we are changing in Y, we need to see how much X is changing. Because
    // of this, we need the inverse of the slope instead.
    const float inverseSlope1 = (float)(x1 - x0) / (y1 - y0);
    const float inverseSlope2 = (float)(x2 - x0) / (y2 - y0);

    // xStart and xEnd from the top vertext (x0,y0);
    float xStart = x0;
    float xEnd = x0;
    for (int y = y0; y < y2; y++) {
        drawLine(xStart, y, xEnd, y, color);
        xStart += inverseSlope1;
        xEnd += inverseSlope2;
    }
}

void fillFlatTopTriangle(int x1, int y1, int mx, int my, int x2, int y2, uint32_t color) {}

// todo(hector) - get cool comment that shows the triangle we are drawing here
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // sort the vertices by y-coodinates ascending (y0 < y1 < y2).
    if (y0 > y1) {
        intSwap(&y0, &y1);
        intSwap(&x0, &x1);
    }
    if (y1 > y2) {
        intSwap(&y1, &y2);
        intSwap(&x1, &x2);
    }
    if (y0 > y1) {
        intSwap(&y0, &y1);
        intSwap(&x0, &x1);
    }

    // calculate the new vertext (Mx, My) using the triangle similarity
    const int My = y1;
    const int Mx = (float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0) + x0;

    // draw flat-bottom triangle
    fillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);
    // draw flat-top triangle
    fillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
}
