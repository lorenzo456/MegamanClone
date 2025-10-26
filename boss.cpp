#include "boss.h"
#include <iostream>
#include "bullet.h"
#include <cmath>

Boss::Boss(Vector2 position, Vector2 size, std::vector<Bullet*>& bullets)
    : position(position), size(size), bullets(bullets)
{
    laser2 = LoadSound("../Sounds/Laser/laser2.wav");
    hitSound = LoadSound("../Sounds/Hit/hit.wav");
    characterIdle = LoadTexture("../Sprites/Boss/boss.png");
    Init();
}

void Boss::Init(bool reset)
{
    currentPhase = MovementPhase::TOP_RIGHT;
    currentShootPhase = ShootPhase::TRAVEL;
    speed = 2.0f; // adjust as needed
    isHit = false;
    health = 15;
    if (reset)
    {
        isActive = false;
        initialized = false;
    }
    else{
        isActive = true;
        initialized = true;
    }
    frameWidth = (float)(characterIdle.width/3);   // Sprite one frame rectangle width
    frameHeight = (float)(characterIdle.height/1);           // Sprite one frame rectangle height
    currentFrame = 0;
    currentLine = 0;
    frameCounter = 0;
    frameRec = { 0, 0, frameWidth, frameHeight };
}

void Boss::Update() 
{

    if (isHit) {
        health--;
        PlaySound(hitSound);
        skipFrame = true;
        isHit = false;
    }

    if (health <= 0) {
        isActive = false;
    }

    if (!isActive) {
        return;
    }

    constexpr float SCREEN_WIDTH = 780.0f;
    constexpr float SCREEN_HEIGHT = 420.0f;

    // Shoot only when in a corner
    if (currentShootPhase == ShootPhase::SHOOT)
    {
        static double lastShotTime = -1000.0;
        static int shotsFired = 0;
        double now = GetTime();
        isInvulnerable = false;

        // Check if visually in a corner
        bool inCorner =
            (position.x <= 0.0f && position.y <= 0.0f) ||                 // top-left
            (position.x + size.x >= SCREEN_WIDTH && position.y <= 0.0f) || // top-right
            (position.x <= 0.0f && position.y + size.y >= SCREEN_HEIGHT) || // bottom-left
            (position.x + size.x >= SCREEN_WIDTH && position.y + size.y >= SCREEN_HEIGHT); // bottom-right

        if (inCorner) {
            if (shotsFired == 0) {
                Shoot();
                lastShotTime = now;
                shotsFired = 1;
            } else if (shotsFired == 1 && now - lastShotTime >= 2.0) {
                Shoot();
                shotsFired = 0;
                currentShootPhase = ShootPhase::TRAVEL;
            }
        }
    }

    // Movement logic
    if (currentShootPhase == ShootPhase::TRAVEL)
    {
        isInvulnerable = true;
        switch (currentPhase) {
            case MovementPhase::TOP_RIGHT:
                if (position.x + size.x < SCREEN_WIDTH)
                    position.x += speed;
                else if (position.y > 0.0f)
                    position.y -= speed;

                if (position.x + size.x >= SCREEN_WIDTH && position.y <= 0.0f) {
                    position.x = SCREEN_WIDTH - size.x;
                    position.y = 0.0f;
                    currentPhase = MovementPhase::TOP_LEFT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;

            case MovementPhase::TOP_LEFT:
                if (position.x > 0.0f)
                    position.x -= speed;
                else if (position.y > 0.0f)
                    position.y -= speed;

                if (position.x <= 0.0f && position.y <= 0.0f) {
                    position.x = 0.0f;
                    position.y = 0.0f;
                    currentPhase = MovementPhase::BOTTOM_LEFT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;

            case MovementPhase::BOTTOM_LEFT:
                if (position.x > 0.0f)
                    position.x -= speed;
                else if (position.y + size.y < SCREEN_HEIGHT)
                    position.y += speed;

                if (position.x <= 0.0f && position.y + size.y >= SCREEN_HEIGHT) {
                    position.x = 0.0f;
                    position.y = SCREEN_HEIGHT - size.y;
                    currentPhase = MovementPhase::BOTTOM_RIGHT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;

            case MovementPhase::BOTTOM_RIGHT:
                if (position.x + size.x < SCREEN_WIDTH)
                    position.x += speed;
                else if (position.y + size.y < SCREEN_HEIGHT)
                    position.y += speed;

                if (position.x + size.x >= SCREEN_WIDTH && position.y + size.y >= SCREEN_HEIGHT) {
                    position.x = SCREEN_WIDTH - size.x;
                    position.y = SCREEN_HEIGHT - size.y;
                    currentPhase = MovementPhase::TOP_RIGHT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;
        }
    }
    UpdateFrameRec();
}

void Boss::Shoot() 
{
    constexpr int BULLET_COUNT = 10;
    constexpr float SPREAD = 20.0f;
    constexpr float BULLET_SPEED = 1.0f;

    Vector2 center = { position.x + size.x * 0.5f, position.y + size.y * 0.5f };

    // Determine target based on actual screen position
    Vector2 target;
    constexpr float SCREEN_WIDTH = 780.0f;
    constexpr float SCREEN_HEIGHT = 420.0f;

    if (position.x + size.x >= SCREEN_WIDTH && position.y <= 0.0f) {
        target = { 0.0f, SCREEN_HEIGHT }; // top-right → bottom-left
    } else if (position.x <= 0.0f && position.y <= 0.0f) {
        target = { SCREEN_WIDTH, SCREEN_HEIGHT }; // top-left → bottom-right
    } else if (position.x <= 0.0f && position.y + size.y >= SCREEN_HEIGHT) {
        target = { SCREEN_WIDTH, 0.0f }; // bottom-left → top-right
    } else if (position.x + size.x >= SCREEN_WIDTH && position.y + size.y >= SCREEN_HEIGHT) {
        target = { 0.0f, 0.0f }; // bottom-right → top-left
    } else {
        return; // not in a corner → do not shoot
    }
    
    // Compute normalized direction
    float forwardOffset = 100.0f;
    Vector2 dir = { target.x - center.x, target.y - center.y };
    float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
    if (len != 0.0f) {
        dir.x /= len;
        dir.y /= len;
    }

    // Perpendicular vector for bullet spread
    Vector2 perp = { -dir.y, dir.x };

for (int i = 0; i < BULLET_COUNT; ++i) {
    float offset = ((i - (BULLET_COUNT-1)/2.0f) * SPREAD);

    // spawn position: center + forward offset along dir + perpendicular spread
    Vector2 spawnPos = {
        center.x + dir.x * forwardOffset + perp.x * offset,
        center.y + dir.y * forwardOffset + perp.y * offset
    };

    Vector2 velocity = { dir.x * BULLET_SPEED, dir.y * BULLET_SPEED };

    Bullet* bullet = new Bullet(spawnPos, {10,5}, velocity, 10.0f);
    bullet->isActive = true;
    bullets.push_back(bullet);
}

    PlaySound(laser2);
}

void Boss::UpdateFrameRec()
{
    frameCounter++;
    if (frameCounter >= 10) // Change frame every 10 updates
    {
        currentFrame++;
        if (currentFrame >= 3) // Assuming 3 frames in the sprite sheet
        {
            currentFrame = 0;
        }
        frameCounter = 0;
    }
    frameRec.x = frameWidth * currentFrame;
    frameRec.y = frameHeight * currentLine;
}

void Boss::Draw() 
{
    // DrawRectangle(position.x, position.y, size.x, size.y, RED);
    if (skipFrame)
    {
        skipFrame = false;
        return;
    }
    
    Rectangle sourceRec = frameRec; // portion of the texture to draw
    Rectangle destRec = { position.x, position.y, frameRec.width, frameRec.height }; // rectangle on screen
    Vector2 origin = { 0, 0 }; // no rotation
    DrawTexturePro(characterIdle, sourceRec, destRec, origin, 0.0f, WHITE);

    // DrawTextureRec(characterIdle, { frameRec.x, frameRec.y, frameRec.width, frameRec.height }, position, WHITE);
}

Rectangle Boss::GetRectangle() 
{
    return { position.x, position.y, size.x, size.y };
}
