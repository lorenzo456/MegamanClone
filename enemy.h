#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <vector>  // Use <vector> instead of "vector"
#include "bullet.h"

class Enemy {
public:
    Enemy(Vector2 position, Vector2 size, float speed, int enemyType, std::vector<Bullet*>&bullets);

    void Update();
    void Draw();
    Rectangle GetRectangle();
    
    int direction;
    int health = 5;
    bool isActive;
    bool isHit;
    Vector2 position;
    Vector2 size;

private:
    float speed;
    float gravity = 2.0f;
    float CurrentVelocityY = 0.0f;
    float jumpVelocity = -30.0f;
    int enemyType = 0;
    std::vector<Bullet*>& bullets; 
    void Shoot();
};

#endif
