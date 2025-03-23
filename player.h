#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "vector"

class Player {
public:
    Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity);

    void Update();
    void Draw();
    void OnHit(int damage);
    void OnDeath();
    Rectangle GetRectangle();
    
    bool isJumping;
    bool isHit;
    float playerVelocityY;
    int direction;
    Vector2 position;
    Vector2 size;

private:
    int health;
    float speed;
    float gravity;
    float jumpVelocity;
};

#endif // PLAYER_H