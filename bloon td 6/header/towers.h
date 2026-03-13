#pragma once
#include "raylib.h"
#include <vector>
#include "bloons.h"
#include "effects.h"

enum UpgradePath { NONE, SHARP_DARTS, MAGIC_BOLT };

class Tower {
public:
    Vector2 position;
    float range;
    float fireRate; // Seconds between shots
    float timer;    // Tracks time since last shot

    UpgradePath path; // Current upgrade state
    std::vector<Effect> darts;

    // Inside class Tower in towers.h
    static bool IsPlacementValid(Vector2 pos, const std::vector<Tower>& existingTowers, Map& gameMap, Rectangle uiRect);

    Tower(Vector2 pos );
    void Update(std::vector<Bloon>& bloons, int &money);
    void Draw();

    bool IsClicked(Vector2 mousePos);
};
