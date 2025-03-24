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
    Texture2D character;
    Rectangle frameRec;
    float frameWidth;   // Sprite one frame rectangle width
    float frameHeight;           // Sprite one frame rectangle height
    int currentFrame;
    int currentLine;
    int frameCounter;
};

#endif // PLAYER_H