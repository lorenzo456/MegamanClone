#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "vector"

class Enemy {
public:
    Enemy(Vector2 position, Vector2 size, float speed);

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
};

#endif 