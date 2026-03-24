#include "raylib.h"
#include "raymath.h"
#include "maps.h"
#include "pathbuilder.h"
#include "gamemanager.h"
#include "account.h"
#include <string>
#include <vector>

struct MapButton {
    Rectangle rect;
    std::string name;
};

// Helper to convert real window mouse position to our 400x240 virtual space
Vector2 GetVirtualMouse(RenderTexture2D target, float scale) {
    Vector2 mouse = GetMousePosition();
    Vector2 vMouse = { 0 };
    vMouse.x = (mouse.x - (GetScreenWidth() - (400 * scale)) * 0.5f) / scale;
    vMouse.y = (mouse.y - (GetScreenHeight() - (240 * scale)) * 0.5f) / scale;
    vMouse.x = Clamp(vMouse.x, 0, 400);
    vMouse.y = Clamp(vMouse.y, 0, 240);
    return vMouse;
}

int main() {
    // 1. Initialization with Resizable Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(800, 480, "Bloons TD - Flexible Edition"); // Startup size
    SetWindowMinSize(400, 240);
    SetTargetFPS(60);

    // Create the Virtual Canvas
    RenderTexture2D canvas = LoadRenderTexture(400, 240);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

    PlayerAccount acc;
    acc.Load();

    GameState state = MENU;
    Map myMap;
    PathBuilder builder;
    GameManager game;

    std::string mapName = "";
    FilePathList files = { 0 };
    std::vector<MapButton> mapButtons;

    while (!WindowShouldClose()) {
        // Handle Fullscreen Toggle
        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)) ToggleFullscreen();

        // Calculate scaling factor for this frame
        float scale = fminf((float)GetScreenWidth() / 400.0f, (float)GetScreenHeight() / 240.0f);
        Vector2 mouse = GetVirtualMouse(canvas, scale);

        // --- 2. UPDATE LOGIC ---
        if (state == MENU) {
            if (files.paths == NULL || IsKeyPressed(KEY_R)) {
                UnloadDirectoryFiles(files);
                files = LoadDirectoryFiles(".");
                mapButtons.clear();

                int count = 0;
                for (unsigned int i = 0; i < files.count; i++) {
                    if (IsFileExtension(files.paths[i], ".txt")) {
                        std::string fName = GetFileName(files.paths[i]);
                        // Define button area in virtual 400x240 space
                        mapButtons.push_back({ { 15, (float)(60 + (count * 22)), 110, 20 }, fName });
                        count++;
                    }
                }
            }

            for (auto& btn : mapButtons) {
                if (CheckCollisionPointRec(mouse, btn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    myMap.LoadFromFile(btn.name);
                    mapName = btn.name;
                }
            }

            // Shop logic using virtual mouse
            if (!acc.magicTowerUnlocked) {
                Rectangle shopBtn = { 220, 180, 160, 35 };
                if (CheckCollisionPointRec(mouse, shopBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (acc.totalMoney >= 2000) {
                        acc.totalMoney -= 2000;
                        acc.magicTowerUnlocked = true;
                        acc.Save();
                    }
                }
            }

            if (IsKeyPressed(KEY_ONE)) { myMap.Clear(); state = BUILD; }
            if (IsKeyPressed(KEY_TWO) && myMap.points.size() > 1) state = PLAY;
        }
        else if (state == BUILD) {
            // Note: builder.Update should be modified to accept 'mouse' instead of calling GetMousePosition()
            builder.Update(myMap, mouse);

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (mapName.length() < 15)) mapName += (char)key;
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && mapName.length() > 0) mapName.pop_back();

            if (IsKeyPressed(KEY_S) && myMap.points.size() > 1) {
                std::string savePath = mapName;
                if (savePath.find(".txt") == std::string::npos) savePath += ".txt";
                myMap.SaveToFile(savePath);
                UnloadDirectoryFiles(files);
                files.paths = NULL;
                state = MENU;
            }
            if (IsKeyPressed(KEY_ENTER)) state = MENU;
        }
        else if (state == PLAY) {
            game.Update(myMap, state, acc, mouse);
            if (IsKeyPressed(KEY_ESCAPE)) state = MENU;
        }

        // --- 3. DRAWING TO CANVAS ---
        BeginTextureMode(canvas);
        ClearBackground(DARKGRAY);

        if (state == GAMEOVER || state == VICTORY) {
            DrawRectangle(0, 0, 400, 240, Fade(BLACK, 0.8f));
            Color titleColor = (state == GAMEOVER) ? RED : GOLD;
            DrawText(state == GAMEOVER ? "DEFEAT" : "VICTORY", 140, 80, 30, titleColor);
            DrawText("Press [R] to Restart | [M] Menu", 100, 130, 15, RAYWHITE);
        }
        else if (state == MENU) {
            DrawText(TextFormat("Balance: $%i", acc.totalMoney), 10, 10, 15, GOLD);
            DrawRectangle(10, 30, 120, 200, Fade(BLACK, 0.4f));
            DrawText("MAPS", 45, 40, 15, GOLD);

            for (auto& btn : mapButtons) {
                bool isHover = CheckCollisionPointRec(mouse, btn.rect);
                bool isSelected = (btn.name == mapName);
                DrawRectangleRec(btn.rect, isHover ? LIGHTGRAY : (isSelected ? DARKGREEN : GRAY));
                DrawText(btn.name.c_str(), (int)btn.rect.x + 5, (int)btn.rect.y + 3, 12, isSelected ? WHITE : BLACK);
            }

            if (!acc.magicTowerUnlocked) {
                Rectangle shopBtn = { 220, 180, 160, 35 };
                DrawRectangleRec(shopBtn, CheckCollisionPointRec(mouse, shopBtn) ? PURPLE : DARKGRAY);
                DrawText("UNLOCK MAGIC ($2000)", 230, 192, 10, WHITE);
            }

            DrawText("1. DRAW NEW MAP", 180, 60, 18, RAYWHITE);
            DrawText("2. START GAME", 180, 90, 18, (myMap.points.size() > 1 ? GREEN : RED));
            DrawText(TextFormat("Map: %s", mapName.c_str()), 180, 115, 12, LIGHTGRAY);
        }
        else if (state == BUILD) {
            myMap.Draw();
            builder.Draw();
            DrawRectangle(260, 10, 130, 60, Fade(BLACK, 0.7f));
            DrawText("SAVE AS:", 265, 15, 10, GOLD);
            DrawText(mapName.c_str(), 265, 30, 15, WHITE);
            DrawText("[S] Save | [Enter] Exit", 265, 50, 9, GREEN);
        }
        else if (state == PLAY) {
            game.Draw(myMap, mouse);
        }
        EndTextureMode();

        // --- 4. DRAW CANVAS TO SCREEN (SCALING) ---
        BeginDrawing();
        ClearBackground(BLACK); // Letterbox bars
        DrawTexturePro(canvas.texture,
            { 0, 0, (float)canvas.texture.width, (float)-canvas.texture.height },
            { (GetScreenWidth() - (400 * scale)) * 0.5f, (GetScreenHeight() - (240 * scale)) * 0.5f, 400 * scale, 240 * scale },
            { 0, 0 }, 0, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    UnloadDirectoryFiles(files);
    acc.Save();
    CloseWindow();
    return 0;
}