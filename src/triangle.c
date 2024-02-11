//
// Created by Hector Mejia on 12/26/23.
//

#include "triangle.h"

#include "display.h"
#include "swap.h"


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

void drawTexel(
    int x, int y, const uint32_t *texture,
    Vec2 pointA, Vec2 pointB, Vec2 pointC,
    float u0, float v0, float u1, float v1, float u2, float v2
) {
    Vec2 pointP = {.x = x, .y = y};
    Vec3 weights = barycentricWeights(pointA, pointB, pointC, pointP);
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    // perform the interpolation of all UV values using the barycentric weights
    float u = (u0 * alpha) + (u1 * beta) + (u2 * gamma);
    float v = (v0 * alpha) + (v1 * beta) + (v2 * gamma);

    // Map the UV coordinates to the texture
    int textureX = abs((int) (u * textureWidth));
    int textureY = abs((int) (v * textureHeight));

    uint32_t texelIndex = (textureY * textureWidth) + textureX;
    if (texelIndex >= textureWidth * textureHeight) {
        return;
    }
    drawPixel(x, y, texture[texelIndex]);
}

void drawTexturedTriangle(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    uint32_t *texture
) {
    // sort the vertices by y-coordinates ascending (y0 < y1 < y2).
    if (y0 > y1) {
        intSwap(&y0, &y1);
        intSwap(&x0, &x1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }
    if (y1 > y2) {
        intSwap(&y1, &y2);
        intSwap(&x1, &x2);
        floatSwap(&u1, &u2);
        floatSwap(&v1, &v2);
    }
    if (y0 > y1) {
        intSwap(&y0, &y1);
        intSwap(&x0, &x1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }

    // create vector points after we sort the vertices
    Vec2 pointA = {.x = x0, .y = y0};
    Vec2 pointB = {.x = x1, .y = y1};
    Vec2 pointC = {.x = x2, .y = y2};

    // fill up the top half of the triangle
    float inverseSlope1 = 0;
    float inverseSlope2 = 0;
    if (y1 - y0 != 0) {
        inverseSlope1 = (float) (x1 - x0) / (float) abs(y1 - y0);
    }
    if (y2 - y0 != 0) {
        inverseSlope2 = (float) (x2 - x0) / (float) abs(y2 - y0);
    }
    if (y1 - y0 != 0) {
        for (int y = y0; y < y1; y++) {
            int xStart = x1 + (y - y1) * inverseSlope1;
            int xEnd = x0 + (y - y0) * inverseSlope2;
            if (xEnd < xStart) {
                intSwap(&xStart, &xEnd);
            }

            for (int x = (int) xStart; x < (int) xEnd; x++) {
                drawTexel(x, y, texture, pointA, pointB, pointC, u0, v0, u1, v1, u2, v2);
            }
        }
    }

    // fill up the bottom half of the triangle
    inverseSlope1 = 0;
    inverseSlope2 = 0;
    if (y2 - y1 != 0) {
        inverseSlope1 = (float) (x2 - x1) / (float) abs(y2 - y1);
    }
    if (y2 - y0 != 0) {
        inverseSlope2 = (float) (x2 - x0) / (float) abs(y2 - y0);
    }
    if (y2 - y1 != 0) {
        for (int y = y1; y < y2; y++) {
            int xStart = x1 + (y - y1) * inverseSlope1;
            int xEnd = x0 + (y - y0) * inverseSlope2;
            if (xEnd < xStart) {
                intSwap(&xStart, &xEnd);
            }

            for (int x = (int) xStart; x < (int) xEnd; x++) {
                drawTexel(x, y, texture, pointA, pointB, pointC, u0, v0, u1, v1, u2, v2);
            }
        }
    }
}

Vec3 barycentricWeights(Vec2 a, Vec2 b, Vec2 c, Vec2 p) {
    Vec2 ac = vec2_sub(c, a);
    Vec2 ab = vec2_sub(b, a);
    Vec2 pc = vec2_sub(c, p);
    Vec2 pb = vec2_sub(b, p);
    Vec2 ap = vec2_sub(p, a);

    // area of the full parallelogram (triangle ABC) using cross product
    float areaParallelogramABC = ac.x * ab.y - ac.y * ab.x; // 2d cross product, weird right?

    // alpha = area of the parallelogram (triangle PBC) / area of the full parallelogram (triangle ABC)
    float alpha = (pc.x * pb.y - pc.y * pb.x) / areaParallelogramABC;

    // beta = area of the parallelogram (triangle PCA) / area of the full parallelogram (triangle ABC)
    float beta = (ac.x * ap.y - ac.y * ap.x) / areaParallelogramABC;

    // gamma = area of the parallelogram (triangle PAB) / area of the full parallelogram (triangle ABC)
    // float gamma = (ab.x * ap.y - ab.y * ap.x) / areaParallelogramABC;

    // gamma = 1 - alpha - beta
    float gamma = 1.0f - alpha - beta;

    Vec3 result = {.x = alpha, .y = beta, .z = gamma};
    return result;
}

