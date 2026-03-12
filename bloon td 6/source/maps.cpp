#include "maps.h"

Map::Map() {
    // Initialize everything to 0 (Empty)
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            grid[x][y] = 0;
        }
    }
}

bool Map::IsPath(int x, int y) const {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        return grid[x][y] == 1;
    }
    return false;
}

void Map::Update() {
    Vector2 mousePos = GetMousePosition();
    int gridX = (int)mousePos.x / CELL_SIZE;
    int gridY = (int)mousePos.y / CELL_SIZE;

    if (gridX >= 0 && gridX < COLS && gridY >= 0 && gridY < ROWS) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            grid[gridX][gridY] = 1; // Path
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (grid[gridX][gridY] == 0) grid[gridX][gridY] = 2; // Tower
        }
    }
}

void Map::Draw() {
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            int posX = x * CELL_SIZE;
            int posY = y * CELL_SIZE;

            DrawRectangleLines(posX, posY, CELL_SIZE, CELL_SIZE, GRAY);

            if (grid[x][y] == 1) DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, DARKBROWN);
            else if (grid[x][y] == 2) DrawCircle(posX + 20, posY + 20, 15, BLUE);
        }
    }
}