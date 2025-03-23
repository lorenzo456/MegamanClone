#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"
#include <vector>

class Obstacle
{

public:
    Obstacle(Vector2 position, Vector2 size, std::vector<Obstacle*> &obstacles);
    void Draw();
    Rectangle GetRectangle() const;
    Vector2 position;
    Vector2 size;
private:
    std::vector<Obstacle*> obstacles;
};


#endif // OBSTACLE_H