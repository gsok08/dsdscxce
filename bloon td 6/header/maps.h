#pragma once
#include "raylib.h"
#include <vector> // CRITICAL: You need this for std::vector
#include <string>
#include <fstream>

class Map {
public:
    std::vector<Vector2> points; // <--- Add this line here
    Vector2 startPoint;

    Map();
    void AddPoint(Vector2 point);
    void Draw();
    void Update();
    void Clear();

    void SaveToFile(std::string filename);
    bool LoadFromFile(std::string filename);
};