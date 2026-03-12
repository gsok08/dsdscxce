#pragma once
#include "raylib.h"

class Effect {
public:
    Vector2 position;
    Vector2 velocity;
    bool active;

    Effect(Vector2 pos, Vector2 target);
    void Update();
    void Draw();
};