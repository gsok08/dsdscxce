#include "effects.h"
#include "raymath.h"

Effect::Effect(Vector2 pos, Vector2 target) {
    position = pos;
    active = true;

    // 1. Direction toward the Bloon
    Vector2 direction = Vector2Subtract(target, pos);
    direction = Vector2Normalize(direction);

    // 2. High speed for a "snappy" dart feel
    float speed = 12.0f;
    velocity = Vector2Scale(direction, speed);
}

void Effect::Update() {
    if (!active) return;

    position = Vector2Add(position, velocity);

    // Bounds check
    if (position.x < -100 || position.x > 900 || position.y < -100 || position.y > 600) {
        active = false;
    }
}

void Effect::Draw() {
    if (!active) return;

    // 3. The "Dart" Geometry
    // We define a direction vector based on velocity to find the tail
    Vector2 dir = Vector2Normalize(velocity);

    // A smaller, thinner stick (12 pixels long, 2 pixels wide)
    float dartLength = 12.0f;
    Vector2 tail = Vector2Subtract(position, Vector2Scale(dir, dartLength));

    // Draw the main body (The "Stick")
    DrawLineEx(tail, position, 2.0f, YELLOW);

    // Draw a tiny "fletching" (the back of the dart) 
    // This makes it look less like a pixel and more like a dart
    Vector2 fletchingPos = Vector2Subtract(position, Vector2Scale(dir, dartLength + 2));
    DrawLineEx(fletchingPos, tail, 1.0f, MAROON);

    // Draw a sharp tip
    DrawCircleV(position, 1, WHITE);
}