#include "towers.h"
#include "raymath.h"
#include "maps.h" 

Tower::Tower(Vector2 pos) {
    position = pos;
    range = 120.0f;
    fireRate = 0.8f;   // Base firing speed
    reloadTimer = 0;   // Initialize timer
    path = NONE;
}

bool Tower::IsPlacementValid(Vector2 pos, const std::vector<Tower>& existingTowers, Map& gameMap, Rectangle uiRect, Rectangle speedRect) {
    float towerRadius = 20.0f;
    float pathWidth = 35.0f;

    // 1. Block buttons
    if (CheckCollisionPointRec(pos, uiRect)) return false;
    if (CheckCollisionPointRec(pos, speedRect)) return false;

    // 2. Check against the Path
    for (size_t i = 0; i < gameMap.points.size(); i++) {
        if (i > 0) {
            if (CheckCollisionPointLine(pos, gameMap.points[i - 1], gameMap.points[i], (int)(pathWidth / 2 + towerRadius))) {
                return false;
            }
        }
        if (CheckCollisionCircles(pos, towerRadius, gameMap.points[i], pathWidth / 2)) {
            return false;
        }
    }

    // 3. Check against existing Towers
    for (const auto& t : existingTowers) {
        if (Vector2Distance(pos, t.position) < (towerRadius * 2)) {
            return false;
        }
    }

    return true;
}

bool Tower::IsClicked(Vector2 mousePos) {
    return CheckCollisionPointCircle(mousePos, position, 15);
}

void Tower::Update(std::vector<Bloon>& bloons, int& money, float speedFactor) {
    // Increment timer multiplied by game speed (1x or 5x)
    reloadTimer += GetFrameTime() * speedFactor;

    // Apply Upgrades
    float effectiveFireRate = fireRate;
    if (path == SHARP_DARTS) effectiveFireRate = fireRate * 0.5f; // Faster
    if (path == MAGIC_BOLT)  range = 200.0f;

    // 1. Shooting Logic
    if (reloadTimer >= effectiveFireRate) {
        for (auto& b : bloons) {
            if (!b.active) continue;

            if (Vector2Distance(position, b.position) <= range) {
                darts.push_back(Effect(position, b.position));
                reloadTimer = 0; // Reset
                break;
            }
        }
    }

    // 2. Projectile Update & Collision
    for (size_t i = 0; i < darts.size(); i++) {
        if (!darts[i].active) continue;

        darts[i].Update(); // Projectiles usually move independently of game speed, 
        // but you can pass speedFactor here too if you want fast darts!

        for (auto& b : bloons) {
            if (b.active && darts[i].active) {
                if (CheckCollisionCircles(darts[i].position, 5, b.position, 15)) {

                    int damage = (path == SHARP_DARTS) ? 2 : 1;
                    b.hp -= damage;
                    darts[i].active = false;

                    if (b.hp <= 0) {
                        b.active = false;
                        money += 10;
                    }
                }
            }
        }
    }

    // 3. Cleanup
    for (int i = (int)darts.size() - 1; i >= 0; i--) {
        if (!darts[i].active) darts.erase(darts.begin() + i);
    }
}

void Tower::Draw() {
    Color towerColor = BLUE;
    if (path == SHARP_DARTS) towerColor = DARKGREEN;
    if (path == MAGIC_BOLT)  towerColor = PURPLE;

    DrawCircleV(position, 15, towerColor);
    DrawCircleLines(position.x, position.y, range, Fade(GRAY, 0.2f));

    for (auto& dart : darts) dart.Draw();
}