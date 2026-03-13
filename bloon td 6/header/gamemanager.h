#pragma once
#include <vector>
#include "raylib.h"
#include "bloons.h"
#include "towers.h"
#include "upgrademanager.h"
#include "maps.h"

enum GameState { MENU, BUILD, PLAY, VICTORY, GAMEOVER };

class GameManager {
public:
    // --- COLLECTIONS ---
    std::vector<Bloon> bloons;
    std::vector<Tower> towers;
    UpgradeManager ui;

    // --- GAME STATS ---
    int money;
    int lives;

    // --- WAVE MANAGEMENT ---
    int waveNumber;
    int bloonsToSpawn;
    float spawnTimer;
    float spawnDelay;
    bool waveActive;

    // --- FUNCTIONS ---
    GameManager();

    void Reset() {
        money = 150;
        lives = 20;
        waveNumber = 0;
        bloons.clear(); // Clear all enemies
        towers.clear(); // Clear all towers
        // Add any other resets here
    }

    float gameSpeed = 1.0f; // 1.0 is normal, 5.0 is fast
    Rectangle speedButton = { 650, 370, 130, 40 }; // Above the Build button

    bool placementMode = false;
    Rectangle buildButton = { 650, 420, 130, 40 }; // Bottom right button

    // Updates all logic (Input, Spawning, AI, Collisions)
    void Update(Map& myMap, GameState& currentState);

    // Renders everything in the game world and the UI
    void Draw(Map& myMap);

    // Helper to reset wave variables
    void StartNextWave();
};