#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "vector"
#include "bullet.h"
#include "soundManager.h"

class Player {
public:
    Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity, std::vector<Bullet*> &bullets, SoundManager* soundManager);

    void Update();
    void Draw();
    void OnHit(int damage);
    void OnDeath();
    void Init(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity, int health, int direction, std::vector<Bullet*> &bullets, bool isAlive);
    Rectangle GetRectangle();
    
    bool isJumping;
    bool isHit;
    bool isAlive;
    bool invinsible;
    bool isShooting;
    float playerVelocityY;
    int direction;
    Vector2 position;
    Vector2 size;

private:
    int health;
    int invinsibleFrames;
    int currentInvinsibleFrame;
    float speed;
    float gravity;
    float jumpVelocity;
    Texture2D characterIdle;
    Texture2D idleFrames[11];
    Texture2D walkFrames[11];
    Texture2D fullHeart;
    Texture2D emptyHeart;
    Texture2D characterWalk;
    Texture2D characterShoot;
    Texture2D hearts;
    Rectangle frameRec;

    Sound laser;
    Sound jumpSound;
    Sound hitSound;

    float frameWidth;   // Sprite one frame rectangle width
    float frameHeight;           // Sprite one frame rectangle height
    float heartsFrameWidth;
    float heartsFrameHeight;
    float currentSpeed;
    float shootingTimer;

    int currentFrame;
    int currentLine;
    int frameCounter;
    int deathCount;
    int currentDeathCount;
    std::vector<Bullet*> &bullets;
    SoundManager* soundManager;
};

#endif // PLAYER_H