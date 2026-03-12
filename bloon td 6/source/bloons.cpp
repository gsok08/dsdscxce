#include "bloons.h"
#include "maps.h"
#include "raymath.h" // For Vector2MoveTowards and Vector2Distance

Bloon::Bloon(Vector2 startPos, BloonType t) {
    position = startPos;
    active = true;
    type = t;
    targetPointIndex = 1; // Start by moving toward the second point in the path

    // Set stats
    if (type == RED_BLOON) { hp = 1; speed = 2.0f; }
    else if (type == BLUE_BLOON) { hp = 2; speed = 3.5f; }
    else if (type == LEAD_BLOON) { hp = 4; speed = 1.0f; }
    else { hp = 1; speed = 2.0f; }
}

void Bloon::TakeDamage(int damage, bool isMagic) {
    // 1. Lead Bloon special logic
    if (type == LEAD_BLOON) {
        if (!isMagic) return; // Only magic can hurt Lead

        // If hit by magic, transform into a Blue Bloon
        type = BLUE_BLOON;
        hp = 2;       // Reset HP to Blue layer's health
        speed = 3.5f; // Blue bloons move faster than Lead!
        return;       // Stop here so the damage doesn't double-pop it instantly
    }

    // 2. Standard Layer Popping
    hp -= damage;

    if (hp <= 0) {
        if (type == BLUE_BLOON) {
            // Blue pops into Red
            type = RED_BLOON;
            hp = 1;
            speed = 2.0f;
        }
        else {
            // Red (or anything else) just dies
            active = false;
        }
    }
}

void Bloon::Update(Map& gameMap) {
    if (!active) return;

    // Safety Check: If there's no path or only 1 point, we can't move
    if (gameMap.points.size() < 2 || targetPointIndex >= gameMap.points.size()) {
        active = false;
        return;
    }

    // 1. Get the current target point
    Vector2 target = gameMap.points[targetPointIndex];

    // 2. Move directly toward that target
    // Vector2MoveTowards is a Raylib function: (current, target, maxDistance)
    position = Vector2MoveTowards(position, target, speed);

    // 3. Check if we arrived at the target point
    if (Vector2Distance(position, target) < 1.0f) {
        targetPointIndex++; // Target the next point in the list

        // 4. If we just passed the last point, the Bloon reached the exit!
        if (targetPointIndex >= gameMap.points.size()) {
            active = false;
            // Note: Your main.cpp handles decreasing lives when this happens
        }
    }
}

void Bloon::Draw() {
    if (!active) return;

    Color c = RED;
    if (type == BLUE_BLOON) c = BLUE;
    if (type == LEAD_BLOON) c = DARKGRAY;

    DrawCircleV(position, 15, c);

    // Optional: Draw a small health bar if hp > 1
    if (hp > 1) {
        DrawText(TextFormat("%i", hp), (int)position.x - 5, (int)position.y - 5, 12, WHITE);
    }
}