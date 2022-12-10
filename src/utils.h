#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include "camera.h"
#include "terrain.h"

bool IsPointInsideQuadInScreenSpace(Vector2 point, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
    Camera camera = GetCamera();

    unsigned int screenScale = GetCurrentScreenScale();

#if defined(PLATFORM_WEB)
    screenScale = 1; // mm, yeah
#endif

    //by screen pos
    Vector2 vertex0Screen = GetWorldToScreen(v0, camera);
    vertex0Screen = (Vector2){ vertex0Screen.x / screenScale , vertex0Screen.y / screenScale };
    Vector2 vertex1Screen = GetWorldToScreen(v1, camera);
    vertex1Screen = (Vector2){ vertex1Screen.x / screenScale , vertex1Screen.y / screenScale };
    Vector2 vertex2Screen = GetWorldToScreen(v2, camera);
    vertex2Screen = (Vector2){ vertex2Screen.x / screenScale , vertex2Screen.y / screenScale };
    Vector2 vertex3Screen = GetWorldToScreen(v3, camera);
    vertex3Screen = (Vector2){ vertex3Screen.x / screenScale , vertex3Screen.y / screenScale };

    if (CheckCollisionPointTriangle(point, vertex0Screen, vertex1Screen, vertex2Screen)
        || CheckCollisionPointTriangle(point, vertex0Screen, vertex2Screen, vertex3Screen))
    {
        return true;
    }
    return false;
}

bool IsPointInsideTileInScreenSpace(Vector2 point, Tile* tile, float halfWidth)
{
    bool result = IsPointInsideQuadInScreenSpace(point,
                (Vector3) { tile->position.x - halfWidth, tile->position.y, tile->position.z + halfWidth },
                (Vector3) { tile->position.x - halfWidth, tile->position.y, tile->position.z - halfWidth },
                (Vector3) { tile->position.x + halfWidth, tile->position.y, tile->position.z - halfWidth },
                (Vector3) { tile->position.x + halfWidth, tile->position.y, tile->position.z + halfWidth });

     return result;
}

#endif