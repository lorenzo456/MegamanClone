#include "boss.h"
#include <iostream>
#include "bullet.h"

Boss::Boss(Vector2 position, Vector2 size)
    : position(position), size(size), bullets(bullets)
{
    laser2 = LoadSound("../Sounds/Laser/laser2.wav");
    currentPhase = MovementPhase::TOP_RIGHT;
}

void Boss::Update() 
{
    // Movement logic based on current phase - only switch phase when both axes hit their limits
    constexpr float SCREEN_WIDTH = 780.0f;
    constexpr float BOTTOM_LIMIT = 420.0f; // boss movement bottom limit

    switch (currentShootPhase)
    {
        case ShootPhase::SHOOT:
            {
                static double lastShotTime = -1000.0;
                static int shotsFired = 0;
                double now = GetTime();

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
            break;
        case ShootPhase::PAUSE:
            // Implement pause behavior here
            break;
    }

    
    if (currentShootPhase == ShootPhase::TRAVEL)
    {    
        switch (currentPhase) {
            case MovementPhase::TOP_RIGHT:
                // move horizontally first (right), then vertically (up)
                if (position.x + size.x < SCREEN_WIDTH) {
                    position.x += speed;
                    if (position.x + size.x > SCREEN_WIDTH) position.x = SCREEN_WIDTH - size.x;
                } else if (position.y > 0.0f) {
                    position.y -= speed;
                    if (position.y < 0.0f) position.y = 0.0f;
                }

                if (position.x + size.x >= SCREEN_WIDTH && position.y <= 0.0f) {
                    position.x = SCREEN_WIDTH - size.x;
                    position.y = 0.0f;
                    currentPhase = MovementPhase::TOP_LEFT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;
            case MovementPhase::TOP_LEFT:
                // move horizontally first (left), then vertically (up)
                if (position.x > 0.0f) {
                    position.x -= speed;
                    if (position.x < 0.0f) position.x = 0.0f;
                } else if (position.y > 0.0f) {
                    position.y -= speed;
                    if (position.y < 0.0f) position.y = 0.0f;
                }

                if (position.x <= 0.0f && position.y <= 0.0f) {
                    position.x = 0.0f;
                    position.y = 0.0f;
                    currentPhase = MovementPhase::BOTTOM_LEFT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;
            case MovementPhase::BOTTOM_LEFT:
                // move horizontally first (left), then vertically (down)
                if (position.x > 0.0f) {
                    position.x -= speed;
                    if (position.x < 0.0f) position.x = 0.0f;
                } else if (position.y + size.y < BOTTOM_LIMIT) {
                    position.y += speed;
                    if (position.y + size.y > BOTTOM_LIMIT) position.y = BOTTOM_LIMIT - size.y;
                }

                if (position.x <= 0.0f && position.y + size.y >= BOTTOM_LIMIT) {
                    position.x = 0.0f;
                    position.y = BOTTOM_LIMIT - size.y;
                    currentPhase = MovementPhase::BOTTOM_RIGHT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;
            case MovementPhase::BOTTOM_RIGHT:
                // move horizontally first (right), then vertically (down)
                if (position.x + size.x < SCREEN_WIDTH) {
                    position.x += speed;
                    if (position.x + size.x > SCREEN_WIDTH) position.x = SCREEN_WIDTH - size.x;
                } else if (position.y + size.y < BOTTOM_LIMIT) {
                    position.y += speed;
                    if (position.y + size.y > BOTTOM_LIMIT) position.y = BOTTOM_LIMIT - size.y;
                }

                if (position.x + size.x >= SCREEN_WIDTH && position.y + size.y >= BOTTOM_LIMIT) {
                    position.x = SCREEN_WIDTH - size.x;
                    position.y = BOTTOM_LIMIT - size.y;
                    currentPhase = MovementPhase::TOP_RIGHT;
                    currentShootPhase = ShootPhase::SHOOT;
                }
                break;
        }
    }
    // Shooting logic can be added here
}

void Boss::Shoot() 
{
    PlaySound(laser2);
}


void Boss::Draw() 
{
    DrawRectangle(position.x, position.y, size.x, size.y, RED);
}
Rectangle Boss::GetRectangle() 
{
    return { position.x, position.y, size.x, size.y };
}


