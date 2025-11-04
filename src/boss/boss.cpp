#include "boss.h"
#include <iostream>
#include "bullet.h"
#include <cmath>

Boss::Boss(Vector2 position, Vector2 size, std::vector<Bullet*>& bullets, SoundManager* soundManager)
    : position(position), size(size), bullets(bullets)
{
    for(int i = 0; i < 3; i++) {
        flyingFrames[i] = LoadTexture(TextFormat("/rd/assets/Sprites/boss/Boss%d.png", i));
    }

    this->soundManager = soundManager;
    Init();
}

void Boss::Init(bool reset)
{


    if (reset)
    {
        isActive = false;
        initialized = false;
        return;
    }

    isActive = true;
    initialized = true;
    currentPhase = MovementPhase::TOP_RIGHT;
    currentShootPhase = ShootPhase::TRAVEL;
    speed = 2.0f;
    isHit = false;
    health = 20;

    frameWidth = (float)(characterIdle.width/3);
    frameHeight = (float)(characterIdle.height/1);
    currentFrame = 0;
    currentLine = 0;
    frameCounter = 0;
    frameRec = { 0, 0, frameWidth, frameHeight };
}

void Boss::Update() 
{
    if (!isActive) {
        return;
    }

    if (isHit) {
        health--;
        soundManager->PlayHitSound();
        skipFrame = true;
        isHit = false;
    }

    if (health <= 0) {
        isActive = false;
    }

    constexpr float SCREEN_WIDTH = 640.0f;
    constexpr float SCREEN_HEIGHT = 480.0f;

    auto inCorner = [&](float x, float y) {
        float tolerance = 2.0f;
        return (
            (fabs(position.x - 0.0f) < tolerance && fabs(position.y - 0.0f) < tolerance) ||
            (fabs((position.x + size.x) - SCREEN_WIDTH) < tolerance && fabs(position.y - 0.0f) < tolerance) ||
            (fabs(position.x - 0.0f) < tolerance && fabs((position.y + size.y) - SCREEN_HEIGHT) < tolerance) ||
            (fabs((position.x + size.x) - SCREEN_WIDTH) < tolerance && fabs((position.y + size.y) - SCREEN_HEIGHT) < tolerance)
        );
    };

    if (currentShootPhase == ShootPhase::SHOOT)
    {
        static double lastShotTime = -1000.0;
        static int shotsFired = 0;
        double now = GetTime();
        isInvulnerable = false;

        if (inCorner(position.x, position.y)) {
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


    if (currentShootPhase == ShootPhase::TRAVEL)
    {
        isInvulnerable = true;

        switch (currentPhase) {
            case MovementPhase::TOP_RIGHT:
                if (position.x + size.x < SCREEN_WIDTH)
                    position.x += speed;
                else if (position.y > 0.0f)
                    position.y -= speed;

                if (inCorner(SCREEN_WIDTH - size.x, 0.0f)) {
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

                if (inCorner(0.0f, 0.0f)) {
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

                if (inCorner(0.0f, SCREEN_HEIGHT - size.y)) {
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

                if (inCorner(SCREEN_WIDTH - size.x, SCREEN_HEIGHT - size.y)) {
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
    constexpr float SPREAD = 35.0f;
    constexpr float BULLET_SPEED = 1.0f;

    Vector2 center = { position.x + size.x * 0.5f, position.y + size.y * 0.5f };

    constexpr float SCREEN_WIDTH = 640.0f;
    constexpr float SCREEN_HEIGHT = 480.0f;

    Vector2 target;

    if (position.x + size.x >= SCREEN_WIDTH && position.y <= 0.0f) {
        target = { 0.0f, SCREEN_HEIGHT };
    } else if (position.x <= 0.0f && position.y <= 0.0f) {
        target = { SCREEN_WIDTH, SCREEN_HEIGHT };
    } else if (position.x <= 0.0f && position.y + size.y >= SCREEN_HEIGHT) {
        target = { SCREEN_WIDTH, 0.0f };
    } else if (position.x + size.x >= SCREEN_WIDTH && position.y + size.y >= SCREEN_HEIGHT) {
        target = { 0.0f, 0.0f };
    } else {
        return;
    }

    Vector2 dir = { target.x - center.x, target.y - center.y };
    float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
    if (len != 0.0f) {
        dir.x /= len;
        dir.y /= len;
    }

    Vector2 perp = { -dir.y, dir.x };
    float forwardOffset = 100.0f;

    for (int i = 0; i < BULLET_COUNT; ++i) {
        float offset = ((i - (BULLET_COUNT-1)/2.0f) * SPREAD);

        Vector2 spawnPos = {
            center.x + dir.x * forwardOffset + perp.x * offset,
            center.y + dir.y * forwardOffset + perp.y * offset
        };

        Vector2 velocity = { dir.x * BULLET_SPEED, dir.y * BULLET_SPEED };

        Bullet* bullet = new Bullet(spawnPos, {10,5}, velocity, 10.0f);
        bullet->isActive = true;
        bullets.push_back(bullet);
    }

    soundManager->PlayLaser2Sound();
}

void Boss::UpdateFrameRec()
{
    frameCounter++;
    if (frameCounter >= 10)
    {
        currentFrame++;
        if (currentFrame >= 3)
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
    if (skipFrame)
    {
        skipFrame = false;
        return;
    }
    DrawTexture(flyingFrames[currentFrame], position.x, position.y, WHITE);
}

Rectangle Boss::GetRectangle() 
{
    return { position.x, position.y, size.x, size.y };
}
