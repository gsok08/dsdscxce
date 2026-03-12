#pragma once
#include "raylib.h"

class Map;

class Bloon {
public:
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    bool active;

    // Constructor: Needs a starting position
    Bloon(Vector2 startPos);

    void Update(Map& gameMap); // Pass the map so the bloon can "see"
    void Draw();   // Render the bloon
};