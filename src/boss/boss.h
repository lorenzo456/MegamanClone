#ifndef BOSS_H
#define BOSS_H

#include "raylib.h"
#include <vector>  
#include "bullet.h"
#include <iostream>
#include "soundManager.h"

class Boss {
public:
    Boss(Vector2 position, Vector2 size, std::vector<Bullet*>&bullets, SoundManager* soundManager);

    void Update();
    void Draw();
    Rectangle GetRectangle();
    void Init(bool reset = false);
    
    int health = 15;
    Vector2 position;
    Vector2 size;
    bool isHit = false;
    bool isActive = true;
    bool isInvulnerable = false;
    bool initialized = false;

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
    void UpdateFrameRec();
    // Sound hitSound;
    // Sound laser2;
    Texture2D characterIdle;
    Texture2D flyingFrames[3];
    float frameWidth; 
    float frameHeight;          
    int currentFrame;
    int currentLine;
    int frameCounter;
    bool skipFrame = false;
    Rectangle frameRec;
    SoundManager* soundManager;

};

#endif
