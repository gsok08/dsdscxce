#include "pathbuilder.h"

void PathBuilder::Update(Map& myMap) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        myMap.AddPoint(GetMousePosition());
    }
    if (IsKeyPressed(KEY_R)) {
        myMap.Clear();
    }
}

void PathBuilder::Draw() {
    DrawText("MODE: PATH BUILDING", 10, 10, 20, YELLOW);
    DrawText("L-Click: Add Point | R: Reset | ENTER: Done", 10, 40, 18, GRAY);
}