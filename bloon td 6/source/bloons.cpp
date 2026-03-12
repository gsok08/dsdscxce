#include "bloons.h"
#include "maps.h" // We must include the real Map.h here to use its functions

Bloon::Bloon(Vector2 startPos) {
    position = startPos;
    direction = { 0, 0 }; // Start still, let Update find the path
    speed = 2.0f;
    active = true;
}

void Bloon::Update(Map& gameMap) {
    if (!active) return;

    // 1. If the bloon is standing still, find a direction!
    if (direction.x == 0 && direction.y == 0) {
        int gx = (int)(position.x / CELL_SIZE);
        int gy = (int)(position.y / CELL_SIZE);

        if (gameMap.IsPath(gx + 1, gy)) direction = { 1, 0 };
        else if (gameMap.IsPath(gx - 1, gy)) direction = { -1, 0 };
        else if (gameMap.IsPath(gx, gy + 1)) direction = { 0, 1 };
        else if (gameMap.IsPath(gx, gy - 1)) direction = { 0, -1 };
    }

    // 2. Normal Movement
    position.x += direction.x * speed;
    position.y += direction.y * speed;

    // 3. Grid Logic (Snapping)
    int gridX = (int)(position.x / CELL_SIZE);
    int gridY = (int)(position.y / CELL_SIZE);
    float centerX = (gridX * CELL_SIZE) + 20;
    float centerY = (gridY * CELL_SIZE) + 20;

    if (CheckCollisionCircles(position, 2, { centerX, centerY }, 2)) {
        position = { centerX, centerY }; // Snap to center

        // LOOK AHEAD: Check if path continues in current direction
        if (!gameMap.IsPath(gridX + (int)direction.x, gridY + (int)direction.y)) {
            // If it DOESN'T continue, find a new turn
            if (direction.x != 0) { // Moving horizontal? Look vertical.
                if (gameMap.IsPath(gridX, gridY + 1)) direction = { 0, 1 };
                else if (gameMap.IsPath(gridX, gridY - 1)) direction = { 0, -1 };
                else direction = { 0,0 }; // Path ended
            }
            else if (direction.y != 0) { // Moving vertical? Look horizontal.
                if (gameMap.IsPath(gridX + 1, gridY)) direction = { 1, 0 };
                else if (gameMap.IsPath(gridX - 1, gridY)) direction = { -1, 0 };
                else direction = { 0,0 }; // Path ended
            }
        }
    }
}
void Bloon::Draw() {
    if (active) DrawCircleV(position, 15, RED);
}