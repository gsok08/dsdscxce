#include "upgrademanager.h"

UpgradeManager::UpgradeManager() {
    selectedTower = nullptr;
    menuBox = { 580, 80, 200, 300 }; // Same as in Draw
}

void UpgradeManager::SelectTower(Tower* t) {
    selectedTower = t;
}

void UpgradeManager::CloseMenu() {
    selectedTower = nullptr;
}

void UpgradeManager::HandleInput(int& money) {
    if (selectedTower == nullptr) return;

    Vector2 mPos = GetMousePosition();

    // Upgrade Button 1: Sharp Darts
    Rectangle btn1 = { 590, 140, 180, 50 };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mPos, btn1)) {
        if (money >= 80 && selectedTower->path == NONE) {
            selectedTower->path = SHARP_DARTS;
            money -= 80;
            CloseMenu();
        }
    }

    // Upgrade Button 2: Magic Bolt
    Rectangle btn2 = { 590, 200, 180, 50 };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mPos, btn2)) {
        if (money >= 150 && selectedTower->path == NONE) {
            selectedTower->path = MAGIC_BOLT;
            money -= 150;
            CloseMenu();
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) CloseMenu();
}

void UpgradeManager::Draw() {
    if (selectedTower == nullptr) return;

    // UI Box Position
    Rectangle menuBox = { 580, 80, 200, 300 };
    DrawRectangleRec(menuBox, Fade(BLACK, 0.8f));
    DrawRectangleLinesEx(menuBox, 2, RAYWHITE);

    DrawText("UPGRADES", 610, 100, 20, GOLD);

    // Path 1
    DrawRectangle(590, 140, 180, 50, (selectedTower->path == SHARP_DARTS) ? DARKGREEN : DARKGRAY);
    DrawText("1. Sharp Darts", 600, 150, 15, WHITE);
    DrawText("$80 - Faster/2x Pop", 600, 170, 12, LIGHTGRAY);

    // Path 2
    DrawRectangle(590, 200, 180, 50, (selectedTower->path == MAGIC_BOLT) ? PURPLE : DARKGRAY);
    DrawText("2. Magic Bolt", 600, 210, 15, WHITE);
    DrawText("$150 - Hits Lead", 600, 230, 12, LIGHTGRAY);

    DrawText("[ESC] to Close", 630, 350, 12, GRAY);
}