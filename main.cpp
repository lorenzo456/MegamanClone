#include "raylib.h"
#include <iostream>
#include <algorithm>
#include "player.h"
#include "obstacle.h"
#include "bullet.h"
using namespace std;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void DrawBackground(Obstacle obstacles[], int size)
{
    for (int i = 0; i < size; i++)
    {
        obstacles[i].Draw();
    }
}

void DrawBullets(std::vector<Bullet*>& bullets)
{
    for (Bullet* bullet : bullets)
    {
        bullet->draw();
    }
}


enum CollisionDirection {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

CollisionDirection CheckCollisionRecsDirection(Rectangle rec1, Rectangle rec2)
{
    if (rec1.x < rec2.x + rec2.width &&
        rec1.x + rec1.width > rec2.x &&
        rec1.y < rec2.y + rec2.height &&
        rec1.y + rec1.height > rec2.y)
    {
        float overlapLeft = (rec1.x + rec1.width) - rec2.x;
        float overlapRight = (rec2.x + rec2.width) - rec1.x;
        float overlapTop = (rec1.y + rec1.height) - rec2.y;
        float overlapBottom = (rec2.y + rec2.height) - rec1.y;

        bool fromLeft = overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom;
        bool fromRight = overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom;
        bool fromTop = overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight;
        bool fromBottom = overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight;

        if (fromLeft) return LEFT;
        if (fromRight) return RIGHT;
        if (fromTop) return TOP;
        if (fromBottom) return BOTTOM;
    }
    return NONE;
}

void CheckForCollisions(Player& player, Obstacle obstacles[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (CheckCollisionRecsDirection(player.GetRectangle(), obstacles[i].GetRectangle()) != NONE)
        {
            CollisionDirection collision = CheckCollisionRecsDirection(player.GetRectangle(), obstacles[i].GetRectangle());
            if (collision == 1)
            {
                player.position.y = obstacles[i].position.y - player.size.y;
                player.isJumping = false;
                player.playerVelocityY = 0.0f;
            }
            else if (collision == 2)
            {
                player.position.y = obstacles[i].position.y + obstacles[i].size.y;
                player.isJumping = false;
                player.playerVelocityY = 0.0f;
            }
            else if (collision == 3)
            {
                player.position.x = obstacles[i].position.x - player.size.x;
            }
            else if (collision == 4)
            {
                player.position.x = obstacles[i].position.x + obstacles[i].size.x;
            }
        }
    }
}


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;


    Obstacle obsticles[3] = {
        Obstacle({500, 350}, {100, 50}),
        Obstacle({300, 350}, {150, 50}),
        Obstacle({100, 300}, {100, 50})
    };
    int obstacleCount = sizeof(obsticles) / sizeof(obsticles[0]);
    
    Player player1({0, (float)screenHeight}, {80, 80}, 5.0f, 2.0f, -30.0f);
    std::vector<Bullet*> bullets;

    InitWindow(screenWidth, screenHeight, "MegaMan Example");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        player1.Update();
        if (IsKeyPressed(KEY_E))
        {
            Bullet* tempBullet = new Bullet(player1.position, {10, 5},player1.direction, 10.0f);
            tempBullet->isActive = true;
            bullets.push_back(tempBullet);
        }

        CheckForCollisions(player1, obsticles, obstacleCount);
        for (Bullet* bullet : bullets)
        {
            bullet->update();
        }


    
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            player1.Draw();
            DrawBackground(obsticles, obstacleCount);
            DrawBullets(bullets);
    
        EndDrawing();
        //----------------------------------------------------------------------------------


        //Garbage Collection
        //----------------------------------------------------------------------------------
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet* b) {
            return !b->isActive;
        }), bullets.end());
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


