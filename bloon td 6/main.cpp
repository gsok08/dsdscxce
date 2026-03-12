#include "raylib.h"
#include "maps.h"
#include "PathBuilder.h"
#include "GameManager.h"

enum GameState { MENU, BUILD, PLAY };

int main() {
    InitWindow(800, 480, "Bloons TD - Custom Maps");
    SetTargetFPS(60);

    GameState state = MENU;
    Map myMap;
    PathBuilder builder;
    GameManager game;

    while (!WindowShouldClose()) {
        if (state == MENU) {
            if (IsKeyPressed(KEY_ONE)) state = BUILD;
            if (IsKeyPressed(KEY_TWO) && myMap.points.size() > 1) state = PLAY;
        }
        else if (state == BUILD) {
            builder.Update(myMap);
            if (IsKeyPressed(KEY_ENTER)) state = MENU;
        }
        else if (state == PLAY) {
            game.Update(myMap);
            if (IsKeyPressed(KEY_ESCAPE)) state = MENU;
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        if (state == MENU) {
            // Corrected: Text, X, Y, Size, Color
            DrawText("1. DRAW PATH", 300, 200, 20, WHITE);

            // Choose color based on whether a path exists
            Color playColor = (myMap.points.size() > 1) ? GREEN : RED;
            DrawText("2. START GAME (Needs Path)", 300, 240, 20, playColor);
        }
        else if (state == BUILD) {
            myMap.Draw(); builder.Draw();
        }
        else if (state == PLAY) {
            game.Draw(myMap);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}