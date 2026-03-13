#include "raylib.h"
#include "maps.h"
#include "pathbuilder.h"
#include "gamemanager.h"
#include <string>
#include <vector>

struct MapButton {
    Rectangle rect;
    std::string name;
};

int main() {
    // 1. Initialization
    InitWindow(800, 480, "Bloons TD - Custom Maps");
    SetTargetFPS(60);

    GameState state = MENU;
    Map myMap;
    PathBuilder builder;
    GameManager game;

    std::string mapName = " "; // Default name for saving/loading
    FilePathList files = { 0 };   // Raylib structure for folder scanning
    std::vector<MapButton> mapButtons;

    while (!WindowShouldClose()) {
        // --- 2. UPDATE LOGIC ---
        if (state == MENU) {
            // Scan folder for .txt files if list is empty or R is pressed
            if (files.paths == NULL || IsKeyPressed(KEY_R)) {
                UnloadDirectoryFiles(files);
                files = LoadDirectoryFiles("."); // Scan current directory
                mapButtons.clear();

                int count = 0;
                for (unsigned int i = 0; i < files.count; i++) {
                    if (IsFileExtension(files.paths[i], ".txt")) {
                        std::string fName = GetFileName(files.paths[i]);
                        // Position buttons on the left side
                        mapButtons.push_back({ { 30, (float)(100 + (count * 35)), 200, 30 }, fName });
                        count++;
                    }
                }
            }

            // Click detection for map selection
            Vector2 mPos = GetMousePosition();
            for (auto& btn : mapButtons) {
                if (CheckCollisionPointRec(mPos, btn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    myMap.LoadFromFile(btn.name);
                    mapName = btn.name; // Set as active
                }
            }

            if (IsKeyPressed(KEY_ONE)) {
                myMap.Clear();
                state = BUILD;
            }
            // Only allow play if map is loaded and valid
            if (IsKeyPressed(KEY_TWO) && myMap.points.size() > 1) {
                state = PLAY;
            }
        }
        else if (state == BUILD) {
            builder.Update(myMap);

            // Typing logic for naming the map
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (mapName.length() < 15)) {
                    mapName += (char)key;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && mapName.length() > 0) mapName.pop_back();

            // Save to file logic
            if (IsKeyPressed(KEY_S) && myMap.points.size() > 1) {
                // Ensure filename ends in .txt
                std::string savePath = mapName;
                if (savePath.find(".txt") == std::string::npos) savePath += ".txt";

                myMap.SaveToFile(savePath);

                // Reset file list so it refreshes next time we enter menu
                UnloadDirectoryFiles(files);
                files.paths = NULL;
                state = MENU;
            }
            if (IsKeyPressed(KEY_ENTER)) state = MENU;
        }
        else if (state == PLAY) {
            game.Update(myMap, state);
            if (IsKeyPressed(KEY_ESCAPE)) state = MENU;
        }

        // --- 3. DRAWING LOGIC ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (state == GAMEOVER || state == VICTORY) {
            DrawRectangle(0, 0, 800, 480, Fade(BLACK, 0.7f));
            DrawText(state == GAMEOVER ? "DEFEAT" : "VICTORY", 300, 200, 40, state == GAMEOVER ? RED : GOLD);

            DrawText("Press [R] to Restart", 320, 260, 20, RAYWHITE);
            DrawText("Press [M] for Menu", 320, 290, 20, RAYWHITE);

            if (IsKeyPressed(KEY_R)) {
                game.Reset();
                state = PLAY;
            }
            if (IsKeyPressed(KEY_M)) {
                game.Reset();
                state = MENU;
            }
        }

        if (state == MENU) {
            // UI Sidebar for Map List
            DrawRectangle(20, 60, 230, 390, Fade(BLACK, 0.4f));
            DrawText("MAP BROWSER", 40, 75, 20, GOLD);
            DrawText("(Press R to Refresh)", 40, 425, 12, GRAY);

            for (auto& btn : mapButtons) {
                bool isHover = CheckCollisionPointRec(GetMousePosition(), btn.rect);
                bool isSelected = (btn.name == mapName);

                DrawRectangleRec(btn.rect, isHover ? LIGHTGRAY : (isSelected ? DARKGREEN : GRAY));
                DrawRectangleLinesEx(btn.rect, 1, isSelected ? GREEN : BLACK);
                DrawText(btn.name.c_str(), (int)btn.rect.x + 8, (int)btn.rect.y + 8, 15, isSelected ? WHITE : BLACK);
            }

            // Right Side Menu
            DrawText("1. DRAW NEW MAP", 350, 180, 25, RAYWHITE);

            Color playColor = (myMap.points.size() > 1) ? GREEN : RED;
            DrawText("2. START GAME", 350, 230, 25, playColor);

            DrawText(TextFormat("Active Map: %s", mapName.c_str()), 350, 265, 18, LIGHTGRAY);

            if (myMap.points.size() <= 1) {
                DrawRectangle(350, 300, 350, 40, Fade(MAROON, 0.3f));
                DrawText("Error: Select a valid map to play!", 360, 310, 16, RED);
            }
        }
        else if (state == BUILD) {
            myMap.Draw();
            builder.Draw();

            // Text Input Box for Saving
            DrawRectangle(550, 20, 230, 110, Fade(BLACK, 0.7f));
            DrawText("SAVE AS:", 560, 35, 15, GOLD);
            DrawText(mapName.c_str(), 560, 60, 20, WHITE);
            DrawRectangle(560, 85, 210, 2, GOLD); // Underline
            DrawText("[S] Save | [Enter] Cancel", 560, 95, 12, GREEN);
        }
        else if (state == PLAY) {
            game.Draw(myMap);
        }

        EndDrawing();
    }

    // 4. Cleanup memory
    UnloadDirectoryFiles(files);
    CloseWindow();
    return 0;
}