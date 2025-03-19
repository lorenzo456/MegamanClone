#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "vector"

class Player {
public:
    Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity);

    void Update();
    void Draw();
    Rectangle GetRectangle();
    
    bool isJumping;
    float playerVelocityY;
    int direction;
    Vector2 position;
    Vector2 size;

private:
    float speed;
    float gravity;
    float jumpVelocity;
};

#endif // PLAYER_H