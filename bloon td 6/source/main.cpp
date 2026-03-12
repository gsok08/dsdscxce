#include "raylib.h"
#include "maps.h"
#include "bloons.h"
#include <vector>

int main() {
    InitWindow(800, 440, "Bloons Prototype - Adding Enemies");
    SetTargetFPS(60);

    Map myMap;

    // This list will hold all our active bloons
    std::vector<Bloon> bloons;

    while (!WindowShouldClose()) {
        // --- 1. INPUT ---
        myMap.Update();

        if (IsKeyPressed(KEY_SPACE)) {
            // We will search the WHOLE grid to find the first brown square you drew
            bool spawned = false;
            for (int x = 0; x < COLS && !spawned; x++) {
                for (int y = 0; y < ROWS && !spawned; y++) {
                    if (myMap.grid[x][y] == 1) { // Found a path!
                        float startX = (float)(x * CELL_SIZE + 20);
                        float startY = (float)(y * CELL_SIZE + 20);
                        bloons.push_back(Bloon({ startX, startY }));
                        spawned = true;
                    }
                }
            }
        }

        // --- 2. UPDATE ---
        for (int i = 0; i < bloons.size(); i++) {
            bloons[i].Update(myMap);
        }

        // --- 3. DRAW ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        myMap.Draw();

        for (int i = 0; i < bloons.size(); i++) {
            bloons[i].Draw();
        }

        DrawText("SPACE: Spawn Bloon | L-Click: Path | R-Click: Tower", 10, 415, 20, WHITE);
        DrawText(TextFormat("Bloons on screen: %i", (int)bloons.size()), 10, 10, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}