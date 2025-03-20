#include "enemy.h"
#include "bullet.h"
#include <algorithm>
#include <iostream>
#include <ostream>

Enemy::Enemy(Vector2 position, Vector2 size, float speed, int enemyType, std::vector<Bullet*>& bullets)
    : position(position), size(size), speed(speed), direction(1), health(5), isActive(true), enemyType(enemyType), bullets(bullets) {}

void Enemy::Update() {
    if (isHit) {
        health--;
        isHit = false;
    }

    if (health <= 0) {
        isActive = false;
    }

    if (position.x < 0) position.x = 0;

    // Raylib shoot every 5 seconds
    static float shootTimer = 0.0f;
    shootTimer += GetFrameTime();
    if (shootTimer >= 5.0f) {
        Shoot();
        shootTimer = 0.0f;
        std::cout << "Enemy Shoot" << std::endl;
    }


}

void Enemy::Draw() {
    if (isActive) {
        DrawRectangle(position.x, position.y, size.x, size.y, RED);
    }
}

Rectangle Enemy::GetRectangle() {
    return { position.x, position.y, size.x, size.y };
}

void Enemy::Shoot() {
    // Vector2 spawnPoint = { position.x - size.x, position.y + size.y / 2 };
    Vector2 spawnPoint = {position.x - size.x, position.y + size.y / 2};
    Bullet* tempBullet = new Bullet(spawnPoint, {10, 5}, -1, 10.0f);
    tempBullet->isActive = true;
    bullets.push_back(tempBullet);
}
