#include "enemy.h"
#include "bullet.h"
#include <algorithm>
#include <iostream>
#include <ostream>

Enemy::Enemy(Vector2 position, Vector2 size, float speed, int enemyType, std::vector<Bullet*>& bullets, float shootDelay)
    : position(position), size(size), speed(speed), direction(1), health(5), isActive(true), enemyType(enemyType), bullets(bullets), shootDelay(shootDelay) 
    {
        initShootDelay = static_cast<float>(GetRandomValue(0, 10)) / 10.0f;

    }

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
    shootTimer += GetFrameTime();
    if (shootTimer >= shootDelay + initShootDelay) {
        Shoot();
        shootTimer = 0.0f;
        initShootDelay = 0;
        // std::cout << "Enemy Shoot" << std::endl;
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
    Vector2 spawnPoint = {position.x - size.x + 10, position.y + size.y / 2};
    Bullet* tempBullet = new Bullet({position.x - size.x + 10, position.y + size.y / 2}, {10, 5}, -1, 10.0f);
    tempBullet->isActive = true;
    bullets.push_back(tempBullet);
}
