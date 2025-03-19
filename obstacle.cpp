#include "obstacle.h"

Obstacle::Obstacle(Vector2 position, Vector2 size)
    : position(position), size(size) {}

void Obstacle::Draw() {
    DrawRectangle(position.x, position.y, size.x, size.y, DARKGRAY);
}

Rectangle Obstacle::GetRectangle() const {
    return { position.x, position.y, size.x, size.y };
}