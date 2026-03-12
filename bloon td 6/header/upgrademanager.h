#pragma once
#include "raylib.h"
#include "towers.h"

class UpgradeManager {
public:
    Tower* selectedTower;
    Rectangle menuBox; // Store the box dimensions

    UpgradeManager();

    // New helper function
    bool IsMouseOver() {
        return (selectedTower != nullptr && CheckCollisionPointRec(GetMousePosition(), menuBox));
    }

    void HandleInput(int& money);
    void Draw();
    void SelectTower(Tower* t);
    void CloseMenu();
};
