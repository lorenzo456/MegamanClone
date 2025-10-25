#ifndef BOSS_H
#define BOSS_H

#include "raylib.h"
#include <vector>  
#include "bullet.h"
#include <iostream>

class Boss {
public:
    Boss(Vector2 position, Vector2 size);

    void Update();
    void Draw();
    Rectangle GetRectangle();
    
    int health = 15;
    Vector2 position;
    Vector2 size;

private:
    std::vector<Bullet*>&bullets; 
    enum class MovementPhase {
        TOP_RIGHT,
        TOP_LEFT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
    };
    MovementPhase currentPhase = MovementPhase::TOP_RIGHT;

    enum class ShootPhase {
        SHOOT,
        PAUSE,
        TRAVEL
    };
    ShootPhase currentShootPhase = ShootPhase::TRAVEL;

    float speed = 2.0f;
    int direction = 1; 
    void Shoot();
    Sound laser2;
};

#endif
