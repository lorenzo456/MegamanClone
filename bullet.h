#include "raylib.h"
#ifndef BULLET_H
#define BULLET_H

class Bullet {
public:
    Bullet(Vector2 position, Vector2 size, int direction, float speed);
    void update();
    void draw();
    Rectangle GetRectangle();
    int direction;
    Vector2 position;
    Vector2 size;
    bool isActive;

private:
    float speed;
};

#endif // BULLET_H