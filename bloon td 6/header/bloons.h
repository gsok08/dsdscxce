#pragma once
#include "raylib.h"

// Forward declaration of Map so the compiler knows it exists
class Map;

enum BloonType { RED_BLOON, BLUE_BLOON, LEAD_BLOON };

class Bloon {
public:
    Vector2 position;
    Vector2 direction; // Note: In the point-to-point system, we mostly use position and target
    float speed;
    int hp;
    BloonType type;
    bool active;

    // --- NEW VARIABLE ---
    int targetPointIndex; // Tracks which path point (index in the vector) the bloon is heading toward

    // Constructor: Needs a starting position and a type
    Bloon(Vector2 startPos, BloonType type);

    void Update(Map& gameMap); // Bloon follows the gameMap.points vector
    void Draw();
    void TakeDamage(int damage, bool isMagic = false);
};