#include "gamemanager.h"

GameManager::GameManager() : money(200), lives(20), waveNumber(0), waveActive(false) {}

void GameManager::Update(Map& myMap) {
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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bool clicked = false;
        for (auto& t : towers) {
            if (t.IsClicked(GetMousePosition())) { ui.SelectTower(&t); clicked = true; break; }
        }
        if (!clicked && !ui.IsMouseOver()) ui.CloseMenu();
    }

    // 4. Building
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && money >= 50) {
        towers.push_back(Tower(GetMousePosition()));
        money -= 50;
    }

    // 5. Update Entities
    ui.HandleInput(money);
    for (auto& t : towers) t.Update(bloons, money);
    for (int i = bloons.size() - 1; i >= 0; i--) {
        bloons[i].Update(myMap);
        if (!bloons[i].active) {
            if (bloons[i].hp > 0) lives--; // Escaped!
            bloons.erase(bloons.begin() + i);
        }
    }
}

void GameManager::Draw(Map& myMap) {
    myMap.Draw();
    for (auto& b : bloons) b.Draw();
    for (auto& t : towers) t.Draw();
    ui.Draw();

    // HUD
    DrawRectangle(0, 0, 800, 40, Fade(BLACK, 0.6f));
    DrawText(TextFormat("CASH: $%i", money), 20, 10, 20, GREEN);
    DrawText(TextFormat("LIVES: %i", lives), 200, 10, 20, RED);
    DrawText(TextFormat("WAVE: %i", waveNumber), 380, 10, 20, GOLD);

    if (!waveActive) {
        DrawText("PRESS [SPACE] TO START NEXT WAVE", 250, 200, 20, RAYWHITE);
    }
}
