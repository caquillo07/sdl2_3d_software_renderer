//
// Created by Hector Mejia on 2/11/24.
//

#include <math.h>
#include "clipping.h"
#include "vector.h"

Plane frustumPlanes[NUM_FRUSTUM_PLANES];

///////////////////////////////////////////////////////////////////////////////
// Frustum planes are defined by a point and a normal vector
///////////////////////////////////////////////////////////////////////////////
// Near plane   :  P=(0, 0, znear), N=(0, 0,  1)
// Far plane    :  P=(0, 0, zfar),  N=(0, 0, -1)
// Top plane    :  P=(0, 0, 0),     N=(0, -cos(fovX/2), sin(fov/2))
// Bottom plane :  P=(0, 0, 0),     N=(0, cos(fov/2), sin(fov/2))
// Left plane   :  P=(0, 0, 0),     N=(cos(fov/2), 0, sin(fov/2))
// Right plane  :  P=(0, 0, 0),     N=(-cos(fov/2), 0, sin(fov/2))
///////////////////////////////////////////////////////////////////////////////
//
//           /|\
//         /  | |
//       /\   | |
//     /      | |
//  P*|-->  <-|*|   ----> +z-axis
//     \      | |
//       \/   | |
//         \  | |
//           \|/
//
///////////////////////////////////////////////////////////////////////////////
void initFrustumPlanes(float fovX, float fovY, float zNear, float zFar) {
    float cosHalfFovX = cos(fovX / 2);
    float sinHalfFovX = sin(fovX / 2);
    float cosHalfFovY = cos(fovY / 2);
    float sinHalfFovY = sin(fovY / 2);

    frustumPlanes[LEFT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = cosHalfFovX;
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = sinHalfFovX;

    frustumPlanes[RIGHT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -cosHalfFovX;
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = sinHalfFovX;

    frustumPlanes[TOP_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -cosHalfFovY;
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = sinHalfFovY;

    frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = cosHalfFovY;
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = sinHalfFovY;

    frustumPlanes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, zNear);
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

    frustumPlanes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, zFar);
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

Polygon createPolygonFromTriangle(
    Vec3 v0,
    Vec3 v1,
    Vec3 v2,
    const Texture2 t0,
    const Texture2 t1,
    const Texture2 t2
) {
    Polygon result = {
        .vertices = {v0, v1, v2},
        .textCoords = {t0, t1, t2},
        .numVertices = 3
    };
    return result;
}

float floatLerp(float first, float last, float factorT) {
    return first + (last - first) * factorT;
}

void clipPolygonAgainstPlane(Polygon *polygon, FrustumPlane plane) {
    Vec3 planePoint = frustumPlanes[plane].point;
    Vec3 planeNormal = frustumPlanes[plane].normal;

    Vec3 insideVertices[MAX_NUM_POLY_VERTICES];
    Texture2 insideTextCoords[MAX_NUM_POLY_VERTICES];
    int numInsideVertices = 0;

    Vec3 *currentVertex = &polygon->vertices[0];
    Vec3 *previousVertex = &polygon->vertices[polygon->numVertices - 1];
    Texture2 *currentTextCoord = &polygon->textCoords[0];
    Texture2 *previousTextCoord = &polygon->textCoords[polygon->numVertices - 1];

    float currentDot = 0;
    float previousDot = vec3_dot(vec3_sub(*previousVertex, planePoint), planeNormal);

    while (currentVertex != &polygon->vertices[polygon->numVertices]) {
        currentDot = vec3_dot(vec3_sub(*currentVertex, planePoint), planeNormal);

        // if we changed from inside to outside or vice-versa
        // since the dot product returns a number with the sign of the angle between the two vectors
        // if the sign changes, it means we went from inside/outside the plane
        if (currentDot * previousDot < 0) {
            // if the edge intersects the plane, add the intersection point to the list of inside vertices
            float t = previousDot / (previousDot - currentDot);
            // Vec3 intersectionPoint = vec3_clone(currentVertex);              // I =        Qc
            // intersectionPoint = vec3_sub(intersectionPoint, *previousVertex); // I =       (Qc-Qp)
            // intersectionPoint = vec3_mul(intersectionPoint, t);                // I =      t(Qc-Qp)
            // intersectionPoint = vec3_add(intersectionPoint, *previousVertex); // I = Qp + t(Qc-Qp)
            // this is equivalent to the above 4 lines
            Vec3 intersectionPoint = {
                .x = floatLerp(previousVertex->x, currentVertex->x, t),
                .y = floatLerp(previousVertex->y, currentVertex->y, t),
                .z = floatLerp(previousVertex->z, currentVertex->z, t)
            };

            // use lerp to ger the interpolated U and V texture coordinates
            Texture2 interpolatedTextCoord = {
                .u = floatLerp(previousTextCoord->u, currentTextCoord->u, t),
                .v = floatLerp(previousTextCoord->v, currentTextCoord->v, t)
            };

            insideTextCoords[numInsideVertices] = interpolatedTextCoord;
            insideVertices[numInsideVertices] = intersectionPoint;
            numInsideVertices++;
        }

        // if current point is inside the plane
        if (currentDot > 0) {
            insideTextCoords[numInsideVertices] = texture2_clone(currentTextCoord);
            insideVertices[numInsideVertices] = vec3_clone(currentVertex);
            numInsideVertices++;
        }

        previousDot = currentDot;
        previousVertex = currentVertex;
        previousTextCoord = currentTextCoord;
        currentVertex++;
        currentTextCoord++;
    }
    for (int i = 0; i < numInsideVertices; i++) {
        polygon->vertices[i] = insideVertices[i];
        polygon->textCoords[i] = insideTextCoords[i];
    }
    polygon->numVertices = numInsideVertices;
}


void clipPolygon(Polygon *polygon) {
    clipPolygonAgainstPlane(polygon, LEFT_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, RIGHT_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, TOP_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, BOTTOM_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, NEAR_FRUSTUM_PLANE);
    clipPolygonAgainstPlane(polygon, FAR_FRUSTUM_PLANE);
}

void createTrianglesFromPolygon(Polygon *polygon, Triangle triangles[], int *numTriangles) {
    for (int i = 0; i < polygon->numVertices - 2; i++) {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vec4_fromVec3(polygon->vertices[index0]);
        triangles[i].points[1] = vec4_fromVec3(polygon->vertices[index1]);
        triangles[i].points[2] = vec4_fromVec3(polygon->vertices[index2]);

        triangles[i].textCoords[0] = texture2_clone(&polygon->textCoords[index0]);
        triangles[i].textCoords[1] = texture2_clone(&polygon->textCoords[index1]);
        triangles[i].textCoords[2] = texture2_clone(&polygon->textCoords[index2]);

    }
    // its always numVertices - 2 because we are creating triangles from a polygon
    *numTriangles = polygon->numVertices - 2;
}

