#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
public:
    Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity);

    void Update();
    void Draw();
    Rectangle GetRectangle() const;

    bool isJumping;
    float playerVelocityY;

private:
    Vector2 position;
    Vector2 size;
    float speed;
    float gravity;
    float jumpVelocity;
};

#endif // PLAYER_H