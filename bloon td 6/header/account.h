#pragma once
#include <fstream>
#include <string>

struct PlayerAccount {
    int totalMoney = 0;
    bool magicTowerUnlocked = false;

    // Save to a local file
    void Save() {
        std::ofstream saveFile("account.dat", std::ios::binary);
        if (saveFile.is_open()) {
            saveFile.write((char*)this, sizeof(PlayerAccount));
            saveFile.close();
        }
    }

    // Load from local file
    void Load() {
        std::ifstream loadFile("account.dat", std::ios::binary);
        if (loadFile.is_open()) {
            loadFile.read((char*)this, sizeof(PlayerAccount));
            loadFile.close();
        }
    }
};