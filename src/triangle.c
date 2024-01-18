//
// Created by Hector Mejia on 12/26/23.
//

#include "triangle.h"

#include "display.h"

void intSwap(int *a, int *b) {
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // find the two slopes from the two triangle legs
    // because we are changing in Y, we need to see how much X is changing. Because
    // of this, we need the inverse of the slope instead.
    const float inverseSlope1 = (float) (x1 - x0) / (float) (y1 - y0);
    const float inverseSlope2 = (float) (x2 - x0) / (float) (y2 - y0);

    // xStart and xEnd from the top vertex (x0,y0);
    float xStart = (float) x0;
    float xEnd = (float) x0;
    for (int y = y0; y < y2; y++) {
        drawLine((int) xStart, y, (int) xEnd, y, color);
        xStart += inverseSlope1;
        xEnd += inverseSlope2;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // x2,y2 are the bottom tip of the triangle, we should draw upwards?
    const float inverseSlope1 = (float) (x0 - x2) / (float) (y0 - y2);
    const float inverseSlope2 = (float) (x1 - x2) / (float) (y1 - y2);

    float xStart = (float) x2;
    float xEnd = (float) x2;
    for (int y = y2; y >= y0; y--) {
        drawLine((int) xStart, y, (int) xEnd, y, color);
        xStart -= inverseSlope1;
        xEnd -= inverseSlope2;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // sort the vertices by y-coordinates ascending (y0 < y1 < y2).
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

    if (y1 == y2) {
        // we can just draw this one
        fillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        fillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        // calculate the new vertex (Mx, My) using the triangle similarity
        const int My = y1;
        const int Mx = (float) ((x2 - x0) * (y1 - y0)) / (float) (y2 - y0) + x0; // NOLINT(*-narrowing-conversions)

        // draw flat-bottom triangle
        fillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);
        // draw flat-top triangle
        fillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
    }
}
