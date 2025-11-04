#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <vector>  // Use <vector> instead of "vector"
#include "bullet.h"
#include "soundManager.h"

class Enemy {
public:
    Enemy(Vector2 position, Vector2 size, float speed, int enemyType, std::vector<Bullet*>&bullets, float shootDelay, std::vector<Enemy*> &enemies, SoundManager* soundManager);

    void Update();
    void Draw();
    Rectangle GetRectangle();
    void FollowPlayer(Vector2 playerDirection);
    
    int direction;
    int health = 5;
    int enemyType = 0;
    bool isActive;
    bool isHit;
    bool follow;
    float velocityY;
    Texture2D characterIdle;
    Texture2D characterWalk;
    Texture2D characterShoot;
    Texture2D idleFrames[11];
    Texture2D walkFrames[11];
    Texture2D testTexture;
    Rectangle frameRec;
    Vector2 position;
    Vector2 size;

private:
    float speed;
    float gravity = 2.0f;
    float CurrentVelocityY = 0.0f;
    float jumpVelocity = -30.0f;
    float shootTimer = 0.0f;
    float shootDelay;
    float initShootDelay;
    float frameWidth; 
    float frameHeight;          
    float heartsFrameWidth;
    float heartsFrameHeight;
    int currentFrame;
    int currentLine;
    int frameCounter;
    float currentSpeed;
    float shootingTimer;
    bool isShooting;
    SoundManager* soundManager;

    // Sound laser2;
    // Sound hitSound;
    Vector2 bulletSpawnpoint;    
    std::vector<Bullet*>&bullets; 
    std::vector<Enemy*>&enemies;
    void Shoot();
};

#endif
