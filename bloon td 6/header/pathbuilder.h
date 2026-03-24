#pragma once
#include "raylib.h"
#include "maps.h"

class PathBuilder {
public:
    void Update(Map& myMap, Vector2 mouse);
    void Draw();
};
