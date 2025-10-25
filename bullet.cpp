#include "bullet.h"

Bullet::Bullet(Vector2 position, Vector2 size, int direction, float speed)
    : position(position), size(size), direction(direction), speed(speed), isActive(false) {
        vecDirection = {static_cast<float>(direction), 0.0f};
    }

Bullet::Bullet(Vector2 position, Vector2 size, Vector2 vecDirection, float speed)
    : position(position), size(size), vecDirection(vecDirection), speed(speed), isActive(false) {}

void Bullet::update() 
{
    if (vecDirection.x != 0 || vecDirection.y != 0)
        {
            position.x += speed * vecDirection.x;
            position.y += speed * vecDirection.y;
        }
    else
        {
            position.x += speed * vecDirection.x;
        }

    if (position.x > GetScreenWidth() || position.x < 0)
    {
        isActive = false;
    }
}  

void Bullet::draw() {
    if(isActive){
        DrawRectangle(position.x, position.y, size.x, size.y, RED);
    }
}

Rectangle Bullet::GetRectangle() {
    return { position.x, position.y, size.x, size.y };
}