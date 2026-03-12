#pragma once
#include <vector>
#include "raylib.h"
#include "bloons.h"
#include "towers.h"
#include "upgrademanager.h"
#include "maps.h"

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

    // Updates all logic (Input, Spawning, AI, Collisions)
    void Update(Map& myMap);

    // Renders everything in the game world and the UI
    void Draw(Map& myMap);

    // Helper to reset wave variables
    void StartNextWave();
};