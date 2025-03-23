#include "obstacle.h"
#include <vector>

Obstacle::Obstacle(Vector2 position, Vector2 size, std::vector<Obstacle*> &obstacles)
    : position(position), size(size), obstacles(obstacles)

    {
        obstacles.push_back(this);
    }

void Obstacle::Draw() {
    DrawRectangle(position.x, position.y, size.x, size.y, DARKGRAY);
}

Rectangle Obstacle::GetRectangle() const {
    return { position.x, position.y, size.x, size.y };
}