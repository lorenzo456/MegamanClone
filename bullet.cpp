#include "bullet.h"

Bullet::Bullet(Vector2 position, Vector2 size, int direction, float speed)
    : position(position), size(size), direction(direction), speed(speed), isActive(false) {}

void Bullet::update() 
{
    position.x += speed * direction; 

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