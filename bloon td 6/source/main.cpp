#include "raylib.h"
#include "maps.h"
#include "pathbuilder.h"
#include "gamemanager.h"
#include <string>
#include <vector>

enum GameState { MENU, BUILD, PLAY };

struct MapButton {
    Rectangle rect;
    std::string name;
};

int main() {
    // 1. Setup
    InitWindow(800, 480, "Bloons TD - Custom Maps");
    SetTargetFPS(60);

    GameState state = MENU;
    Map myMap;
    PathBuilder builder;
    GameManager game;

    std::string mapName = "map1"; // The active map name
    FilePathList files = { 0 };   // For the clickable list
    std::vector<MapButton> mapButtons;

    while (!WindowShouldClose()) {
        // --- 2. UPDATE LOGIC ---
        if (state == MENU) {
            // Load file list if empty or if 'R' is pressed to refresh
            if (files.paths == NULL || IsKeyPressed(KEY_R)) {
                UnloadDirectoryFiles(files);
                files = LoadDirectoryFiles("."); // Scan current folder
                mapButtons.clear();
                int count = 0;
                for (unsigned int i = 0; i < files.count; i++) {
                    if (IsFileExtension(files.paths[i], ".txt")) {
                        std::string fName = GetFileName(files.paths[i]);
                        mapButtons.push_back({ { 40, (float)(100 + (count * 40)), 180, 30 }, fName });
                        count++;
                    }
                }
            }

            // Check for clicks on the Map List
            Vector2 mPos = GetMousePosition();
            for (auto& btn : mapButtons) {
                if (CheckCollisionPointRec(mPos, btn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    myMap.LoadFromFile(btn.name);
                    mapName = btn.name; // Set this as the active map
                }
            }

            if (IsKeyPressed(KEY_ONE)) {
                myMap.Clear();
                state = BUILD;
            }
            if (IsKeyPressed(KEY_TWO) && myMap.points.size() > 1) {
                state = PLAY;
            }
        }
        else if (state == BUILD) {
            builder.Update(myMap);

            // Handle typing for the map name
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (mapName.length() < 15)) {
                    mapName += (char)key;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && mapName.length() > 0) mapName.pop_back();

            // Save and return
            if (IsKeyPressed(KEY_S) && myMap.points.size() > 1) {
                myMap.SaveToFile(mapName.find(".txt") != std::string::npos ? mapName : mapName + ".txt");
                UnloadDirectoryFiles(files); // Force a refresh of the list next time
                files.paths = NULL;
                state = MENU;
            }
            if (IsKeyPressed(KEY_ENTER)) state = MENU;
        }
        else if (state == PLAY) {
            game.Update(myMap);
            if (IsKeyPressed(KEY_ESCAPE)) state = MENU;
        }

        // --- 3. DRAWING LOGIC ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (state == MENU) {
            // Left Side: Map Selector
            DrawRectangle(20, 60, 220, 380, Fade(BLACK, 0.3f));
            DrawText("SELECT A MAP:", 40, 75, 20, GOLD);
            for (auto& btn : mapButtons) {
                bool hover = CheckCollisionPointRec(GetMousePosition(), btn.rect);
                DrawRectangleRec(btn.rect, hover ? LIGHTGRAY : GRAY);
                if (btn.name == mapName) DrawRectangleLinesEx(btn.rect, 2, GREEN);
                DrawText(btn.name.c_str(), (int)btn.rect.x + 5, (int)btn.rect.y + 8, 15, btn.name == mapName ? DARKGREEN : BLACK);
            }

            // Right Side: Main Controls
            DrawText("1. DRAW NEW PATH", 350, 180, 25, RAYWHITE);

            Color playColor = (myMap.points.size() > 1) ? GREEN : RED;
            DrawText(TextFormat("2. START GAME"), 350, 230, 25, playColor);
            DrawText(TextFormat("Selected: %s", mapName.c_str()), 350, 260, 18, GRAY);

            if (myMap.points.size() <= 1) {
                DrawText("(Select a map from the left or draw one!)", 350, 290, 15, MAROON);
            }
        }
        else if (state == BUILD) {
            myMap.Draw();
            builder.Draw();

            // Naming UI
            DrawRectangle(550, 20, 230, 100, Fade(BLACK, 0.6f));
            DrawText("MAP NAME:", 560, 30, 15, GOLD);
            DrawText(mapName.c_str(), 560, 55, 20, WHITE);
            DrawText("[S] to Save & Exit", 560, 90, 15, GREEN);
        }
        else if (state == PLAY) {
            game.Draw(myMap);
        }

        EndDrawing();
    }

    // 4. Cleanup
    UnloadDirectoryFiles(files);
    CloseWindow();
    return 0;
}
