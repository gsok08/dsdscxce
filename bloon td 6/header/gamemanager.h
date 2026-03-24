#pragma once
#include <vector>
#include "raylib.h"
#include "bloons.h"
#include "towers.h"
#include "upgrademanager.h"
#include "maps.h"
#include "account.h"

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

    // Added 'Vector2 mouse' to the parameters
    void Update(Map& myMap, GameState& currentState, PlayerAccount& acc, Vector2 mouse);
    void Draw(Map& myMap, Vector2 mouse);

    // Helper to reset wave variables
    void StartNextWave();
};

