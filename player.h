#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "vector"
#include "bullet.h"

class Player {
public:
    Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity, std::vector<Bullet*> &bullets);

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
    Texture2D characterIdle;
    Texture2D characterWalk;
    Texture2D hearts;
    Rectangle frameRec;

    Sound laser;
    float frameWidth;   // Sprite one frame rectangle width
    float frameHeight;           // Sprite one frame rectangle height
    float heartsFrameWidth;
    float heartsFrameHeight;
    float currentSpeed;
    int currentFrame;
    int currentLine;
    int frameCounter;
    std::vector<Bullet*> &bullets;
};

#endif // PLAYER_H