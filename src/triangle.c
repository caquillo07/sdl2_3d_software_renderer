//
// Created by Hector Mejia on 12/26/23.
//

#include "triangle.h"

#include "display.h"
#include "swap.h"


Vec2 vec2_fromVec4(Vec4 vec41);

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
    Vec4 pointA, Vec4 pointB, Vec4 pointC,
    Texture2 vertexA_UV, Texture2 vertexB_UV, Texture2 vertexC_UV
    //float u0, float v0, float u1, float v1, float u2, float v2
) {
    Vec2 pointP = {.x = x, .y = y};
    Vec2 a = vec2_fromVec4(pointA);
    Vec2 b = vec2_fromVec4(pointB);
    Vec2 c = vec2_fromVec4(pointC);

    Vec3 weights = barycentricWeights(a, b, c, pointP);
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;



    // perform the interpolation of all U/2 and V/w values using the barycentric weights and a factor of 1/w
    float interpolatedU = ((vertexA_UV.u / pointA.w) * alpha) + ((vertexB_UV.u / pointB.w) * beta) + ((vertexC_UV.u / pointC.w) * gamma);
    float interpolatedV = ((vertexA_UV.v / pointA.w) * alpha) + ((vertexB_UV.v / pointB.w) * beta) + ((vertexC_UV.v / pointC.w) * gamma);

    // in a real project, we could pass this in so its not calculated on each iteration.
    // This would speed it up quite a bit since division is _slow_.
    float interpolatedReciprocalW = (1 / pointA.w) * alpha + (1 / pointB.w) * beta + (1 / pointC.w) * gamma;

    interpolatedU /= interpolatedReciprocalW;
    interpolatedV /= interpolatedReciprocalW;

    // Map the UV coordinates to the texture
    int textureX = abs((int) (interpolatedU * textureWidth));
    int textureY = abs((int) (interpolatedV * textureHeight));

    uint32_t texelIndex = (textureY * textureWidth) + textureX;
    if (texelIndex >= textureWidth * textureHeight) {
        return;
    }
    drawPixel(x, y, texture[texelIndex]);
}

void drawTexturedTriangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    uint32_t *texture
) {
    // sort the vertices by y-coordinates ascending (y0 < y1 < y2).
    if (y0 > y1) {
        intSwap(&y0, &y1);
        intSwap(&x0, &x1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
        floatSwap(&z0, &z1);
        floatSwap(&w0, &w1);
    }
    if (y1 > y2) {
        intSwap(&y1, &y2);
        intSwap(&x1, &x2);
        floatSwap(&u1, &u2);
        floatSwap(&v1, &v2);
        floatSwap(&z1, &z2);
        floatSwap(&w1, &w2);
    }
    if (y0 > y1) {
        intSwap(&y0, &y1);
        intSwap(&x0, &x1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
        floatSwap(&z0, &z1);
        floatSwap(&w0, &w1);
    }

    // create vector points after we sort the vertices
    Vec4 pointA = {.x = x0, .y = y0, .z = z0, .w = w0};
    Vec4 pointB = {.x = x1, .y = y1, .z = z1, .w = w1};
    Vec4 pointC = {.x = x2, .y = y2, .z = z2, .w = w2};
    Texture2 vertexA_UV = {.u = u0, .v = v0};
    Texture2 vertexB_UV = {.u = u1, .v = v1};
    Texture2 vertexC_UV = {.u = u2, .v = v2};

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
                drawTexel(x, y, texture, pointA, pointB, pointC, vertexA_UV, vertexB_UV, vertexC_UV);
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
                drawTexel(x, y, texture, pointA, pointB, pointC, vertexA_UV, vertexB_UV, vertexC_UV);
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

