#include "maps.h"

// 1. Constructor: Initialize defaults
Map::Map() {
    startPoint = { -1.0f, -1.0f };
    // The 'points' vector is automatically initialized as empty
}

// 2. AddPoint: Called by PathBuilder to extend the road
void Map::AddPoint(Vector2 point) {
    points.push_back(point);

    // The very first point clicked becomes the spawn location
    if (points.size() == 1) {
        startPoint = point;
    }
}

// 3. Clear: Resets the map so you can draw a new one
void Map::Clear() {
    points.clear();
    startPoint = { -1.0f, -1.0f };
}

// 4. Update: Placeholder for any map animations (like moving water)
void Map::Update() {
    // Currently empty, but useful for future map-specific logic
}

// 5. Draw: Visualizes the road and the start/end points
void Map::Draw() {
    // --- Draw the Road Segments ---
    for (size_t i = 0; i < points.size(); i++) {

        // Draw the connection line to the previous point
        if (i > 0) {
            // Draw a thick line to represent the track
            DrawLineEx(points[i - 1], points[i], 30.0f, LIGHTGRAY);

            // Optional: Draw a thinner line in the middle for a "road" look
            DrawLineEx(points[i - 1], points[i], 2.0f, GRAY);
        }

        // Draw the "Joints" of the path
        DrawCircleV(points[i], 15.0f, LIGHTGRAY);
    }

    // --- Draw UI Markers ---
    if (startPoint.x != -1.0f) {
        // Mark the Entrance
        DrawCircleV(startPoint, 10.0f, GREEN);
        DrawText("START", (int)startPoint.x - 20, (int)startPoint.y - 30, 10, GREEN);

        // If there are at least 2 points, mark the Exit
        if (points.size() > 1) {
            Vector2 endPoint = points.back();
            DrawCircleV(endPoint, 10.0f, RED);
            DrawText("EXIT", (int)endPoint.x - 15, (int)endPoint.y - 30, 10, RED);
        }
    }
}

void Map::SaveToFile(std::string filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << points.size() << "\n";
        for (const auto& p : points) {
            file << p.x << " " << p.y << "\n";
        }
        file.close();
    }
}

bool Map::LoadFromFile(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    points.clear();
    int count;
    file >> count;
    for (int i = 0; i < count; i++) {
        Vector2 p;
        file >> p.x >> p.y;
        AddPoint(p);
    }
    file.close();
    return true;
}
