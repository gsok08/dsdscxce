#include "gamemanager.h"
#include "account.h"

GameManager::GameManager() : money(150), lives(1), waveNumber(0), waveActive(false) {}

void GameManager::Update(Map& myMap , GameState& currentState, PlayerAccount& acc, Vector2 mouse) {
    Vector2 mPos = GetMousePosition();

    if (lives < 1) {
        currentState = GAMEOVER;
    }

    if (waveNumber >= 20 && !waveActive && bloonsToSpawn <= 0) {
        currentState = VICTORY;
    }

    // Inside GameManager::Update when the game ends
    if (currentState == VICTORY || currentState == GAMEOVER) {

        // Logic: 50 coins per wave + bonus for Victory
        int difficultyMultiplier = 1; // You can change this based on map
        int reward = (waveNumber * 50) * difficultyMultiplier;

        if (currentState == VICTORY) reward += 500; // Bonus for winning

        // Add to the permanent account 
        acc.totalMoney += reward;
        acc.Save(); // Save immediately
    }

    // 1. Handle Button Click (Toggle Mode)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mPos, buildButton)) {
            placementMode = !placementMode; // Switch ON/OFF
        }
    }

    // 2. Handle Tower Placement (Only if mode is ON)
    if (placementMode && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (money >= 50) {
            if (Tower::IsPlacementValid(mPos, towers, myMap, buildButton, speedButton)) {
                towers.push_back(Tower(mPos));
                money -= 50;
                placementMode = !placementMode;
                // Optional: placementMode = false; // Turn off after one build
            }
        }
    }

    // 1. Toggle Speed Button
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mPos, speedButton)) {
            gameSpeed = (gameSpeed == 1.0f) ? 5.0f : 1.0f;
        }
    }

    // 2. Apply Speed to Bloon Movement
    for (auto& b : bloons) {
        // Multiply the movement increment by gameSpeed
        // If your bloon has a move function, pass gameSpeed into it
        b.Update(myMap, gameSpeed);
    }

    // 3. Apply Speed to Towers (Cooldowns)
    for (auto& t : towers) {
        // Towers should reload 5x faster in fast mode
        t.Update(bloons, money, gameSpeed);
    }

    // 1. Wave Input
    if (IsKeyPressed(KEY_SPACE) && !waveActive) {
        waveActive = true;
        waveNumber++;

        // Scale number of bloons based on wave (unlimited scaling)
        bloonsToSpawn = 5 + (waveNumber * 4);

        // Make them come out faster as waves progress
        spawnDelay = fmax(0.15f, 1.0f - (waveNumber * 0.05f));
        spawnTimer = spawnDelay; // Spawn the first one immediately
    }

    // 2. Spawning
    if (waveActive && bloonsToSpawn > 0) {
        spawnTimer += GetFrameTime();
        if (spawnTimer >= spawnDelay) {

            // Randomly decide type based on wave difficulty
            BloonType type = RED_BLOON;
            int roll = GetRandomValue(1, 100);

            if (waveNumber == 1) {
                type = RED_BLOON; // Early waves: only red
            }
            else if (waveNumber == 7) {
                type = (roll > 70) ? BLUE_BLOON : RED_BLOON; // Mid waves: mix
            }
            else if (waveNumber == 2) {
                type = LEAD_BLOON;
            }
            else {
                // High waves: chance for Lead bloons
                if (roll > 90) type = LEAD_BLOON;
                else if (roll > 60) type = BLUE_BLOON;
                else type = RED_BLOON;
            }

            if (myMap.points.size() > 0) {
                bloons.push_back(Bloon(myMap.startPoint, type));
            }

            bloonsToSpawn--;
            spawnTimer = 0;
        }
    }

    // --- 3. WAVE COMPLETION ---
    // Check if all bloons are gone and no more are waiting to spawn
    if (waveActive && bloonsToSpawn <= 0 && bloons.size() == 0) {
        waveActive = false;
        money += (50 + waveNumber * 10); // Clear bonus scales with wave
    }

    // 3. Selection & Upgrade UI
    if (!placementMode && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bool clicked = false;
        for (auto& t : towers) {
            if (t.IsClicked(GetMousePosition())) { ui.SelectTower(&t); clicked = true; break; }
        }
        if (!clicked && !ui.IsMouseOver()) ui.CloseMenu();
    }

    

    // 5. Update Entities
    ui.HandleInput(money);
    for (auto& t : towers) t.Update(bloons, money, gameSpeed);
    for (int i = bloons.size() - 1; i >= 0; i--) {
        bloons[i].Update(myMap, gameSpeed);
        if (!bloons[i].active) {
            if (bloons[i].hp > 0) lives--; // Escaped!
            bloons.erase(bloons.begin() + i);
        }
    }
}

void GameManager::Draw(Map& myMap, Vector2 mouse) {
    myMap.Draw();
    for (auto& b : bloons) b.Draw();
    for (auto& t : towers) t.Draw();
    ui.Draw();

    bool canPlace = Tower::IsPlacementValid(mouse, towers, myMap, buildButton,speedButton);

    bool hover = CheckCollisionPointRec(GetMousePosition(), buildButton);
    DrawRectangleRec(buildButton, placementMode ? GREEN : (hover ? LIGHTGRAY : GRAY));
    DrawRectangleLinesEx(buildButton, 2, BLACK);
    DrawText(placementMode ? "CANCEL" : "BUILD TOWER", (int)buildButton.x + 10, (int)buildButton.y + 12, 15, BLACK);
    // Draw Speed Button
    bool speedHover = CheckCollisionPointRec(GetMousePosition(), speedButton);
    DrawRectangleRec(speedButton, gameSpeed > 1.0f ? ORANGE : (speedHover ? LIGHTGRAY : GRAY));
    DrawRectangleLinesEx(speedButton, 2, BLACK);

    const char* speedText = (gameSpeed > 1.0f) ? "SPEED: 5x" : "SPEED: 1x";
    DrawText(speedText, (int)speedButton.x + 25, (int)speedButton.y + 12, 15, BLACK);

    if (placementMode) {
        bool canPlace = Tower::IsPlacementValid(mouse, towers, myMap, buildButton, speedButton);

        // Visual indicator that you are in build mode
        DrawCircleV(mouse, 20, Fade(canPlace ? GREEN : RED, 0.4f));
        DrawText("R-Click to Place", (int)mouse.x + 25, (int)mouse.y, 15, WHITE);
    }
    // HUD

    DrawRectangle(0, 0, 400, 25, Fade(BLACK, 0.6f));
    DrawText(TextFormat("CASH: $%i", money), 20, 10, 20, GREEN);
    DrawText(TextFormat("LIVES: %i", lives), 200, 10, 20, RED);
    DrawText(TextFormat("WAVE: %i", waveNumber), 380, 10, 20, GOLD);

    if (!waveActive) {
        DrawText("PRESS [SPACE] TO START NEXT WAVE", 250, 200, 20, RAYWHITE);
    }
}