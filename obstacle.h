#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"

class Obstacle
{

public:
    Obstacle(Vector2 position, Vector2 size);
    void Draw();
    Rectangle GetRectangle() const;
    Vector2 position;
    Vector2 size;
private:

};


#endif // OBSTACLE_H