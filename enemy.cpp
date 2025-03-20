#include "enemy.h"

Enemy::Enemy(Vector2 position, Vector2 size, float speed)
    : position(position), size(size), speed(speed), direction(1), health(5), isActive(true) {}

void Enemy::Update() {
    if (isHit) {
        health--;
        isHit = false;
    }

    if (health <= 0) {
        isActive = false;
    }
}

void Enemy::Draw() {
    if (isActive){
        DrawRectangle(position.x, position.y, size.x, size.y, RED);
    }
}

Rectangle Enemy::GetRectangle() {
    return { position.x, position.y, size.x, size.y };
}