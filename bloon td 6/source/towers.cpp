#include "towers.h"
#include "raymath.h"
#include "effects.h" // Ensure this matches your filename (Effect.h or effects.h)
#include "maps.h"

Tower::Tower(Vector2 pos) {
    position = pos;
    range = 120.0f;
    fireRate = 0.5f;
    timer = 0;
    path = NONE; // Start as base tower
}

bool Tower::IsPlacementValid(Vector2 pos, const std::vector<Tower>& existingTowers, Map& gameMap, Rectangle uiRect) {
    float towerRadius = 20.0f; // Collision size of the tower
    float pathWidth = 30.0f;   // Should match DrawLineEx width in maps.cpp

    // 1. Check against the Path
    for (size_t i = 0; i < gameMap.points.size(); i++) {
        if (i > 0) {
            // Check if the click is too close to the line segment
            // We use pathWidth/2 + towerRadius to give some "padding"
            if (CheckCollisionPointLine(pos, gameMap.points[i - 1], gameMap.points[i], (int)(pathWidth / 2 + towerRadius))) {
                return false;
            }
        }
        // Also check the "Joints" (the circles at each point)
        if (CheckCollisionCircles(pos, towerRadius, gameMap.points[i], pathWidth / 2)) {
            return false;
        }

        
    }

    // 2. Check against existing Towers (Prevent Overlapping)
    for (const auto& t : existingTowers) {
        if (Vector2Distance(pos, t.position) < (towerRadius * 2)) {
            return false;
        }
    }

    if (CheckCollisionPointRec(pos, uiRect)) {
        return false;
    }

    

    return true;
}


// Helper for the UpgradeManager to detect clicks
bool Tower::IsClicked(Vector2 mousePos) {
    return CheckCollisionPointCircle(mousePos, position, 15);
}

void Tower::Update(std::vector<Bloon>& bloons, int& money) {
    timer += GetFrameTime();

    // --- APPLY UPGRADES ---
    if (path == SHARP_DARTS) {
        fireRate = 0.25f; // Double attack speed
    }
    if (path == MAGIC_BOLT) {
        range = 200.0f;   // Extended range
    }

    // 1. Shooting Logic
    if (timer >= fireRate) {
        for (auto& b : bloons) {
            if (!b.active) continue;

            // Element Logic: Only Magic Bolt hits Lead
            if (b.type == LEAD_BLOON && path != MAGIC_BOLT) continue;

            if (Vector2Distance(position, b.position) <= range) {
                darts.push_back(Effect(position, b.position));
                timer = 0;
                break;
            }
        }
    }

    // 2. Collision Logic
    for (size_t i = 0; i < darts.size(); i++) {
        if (!darts[i].active) continue;

        darts[i].Update();

        for (auto& b : bloons) {
            if (b.active && darts[i].active) {
                if (CheckCollisionCircles(darts[i].position, 4, b.position, 15)) {

                    // Damage Logic: Sharp Darts do 2 damage, others do 1
                    bool magicFlag = (path == MAGIC_BOLT);
                    int damageValue = (path == SHARP_DARTS) ? 2 : 1;

                    // Call the new damage function
                    b.TakeDamage(damageValue, magicFlag);

                    darts[i].active = false; // Destroy the dart

                    if (b.hp <= 0) {
                        b.active = false;
                        money += 10;
                    }
                }
            }
        }
    }

    // 3. Clean up inactive effects
    for (int i = (int)darts.size() - 1; i >= 0; i--) {
        if (!darts[i].active) {
            darts.erase(darts.begin() + i);
        }
    }
}

void Tower::Draw() {
    // Change color based on upgrade path
    Color towerColor = BLUE;
    if (path == SHARP_DARTS) towerColor = DARKGREEN;
    if (path == MAGIC_BOLT)  towerColor = PURPLE;

    DrawCircleV(position, 15, towerColor);

    // Draw subtle range circle
    DrawCircleLines(position.x, position.y, range, Fade(GRAY, 0.2f));

    for (auto& dart : darts) {
        dart.Draw();
    }
}