#pragma once
#include "raylib.h"
#include <vector>
#include "bloons.h"
#include "effects.h"
#include "maps.h" // Needed for Map reference

enum UpgradePath { NONE, SHARP_DARTS, MAGIC_BOLT };

class Tower {
public:
    Vector2 position;
    float range;
    float fireRate;    // Target time to reach
    float reloadTimer; // Current accumulation of time

    UpgradePath path;
    std::vector<Effect> darts;

    // Updated to include both buttons for collision checking
    static bool IsPlacementValid(Vector2 pos, const std::vector<Tower>& existingTowers, Map& gameMap, Rectangle uiRect, Rectangle speedRect);

    Tower(Vector2 pos);
    // money is passed as int& to update global money, speedFactor for 5x speed
    void Update(std::vector<Bloon>& bloons, int& money, float speedFactor);
    void Draw();

    bool IsClicked(Vector2 mousePos);
};