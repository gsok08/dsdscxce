#pragma once
#include "raylib.h"

// Constants
const int CELL_SIZE = 40;
const int COLS = 20;
const int ROWS = 11;

class Map {
public:
    int grid[COLS][ROWS];

    Map(); // Constructor: Sets up the initial grid
    void Update(); // Handles mouse clicks
    void Draw();   // Handles drawing the grid
    bool IsPath(int x, int y) const;
};