#include "raylib.h"
#include <iostream>
#include <algorithm>
#include "player.h"
#include "obstacle.h"
#include "bullet.h"
#include "enemy.h"
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
void DrawEnemies(std::vector<Enemy*>& enemies)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Draw();
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

void CheckBulletCollision(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies)
{
    for (Bullet* bullet : bullets)
    {
        for (Enemy* enemy : enemies)
        {
            if (CheckCollisionRecsDirection(bullet->GetRectangle(), enemy->GetRectangle()) != NONE)
            {
                CollisionDirection collision = CheckCollisionRecsDirection(bullet->GetRectangle(), enemy->GetRectangle());
                if (collision != NONE)
                {
                    bullet->isActive = false;
                    enemy->isHit = true;
                }
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
    std::vector<Enemy*> enemies;

    Enemy enemy1({(float)screenWidth - 80, screenHeight - 80}, {80, 80}, 5.0f, 0);
    enemies.push_back(&enemy1);

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
            Vector2 spawnPoint;
            if (player1.direction == 1) {
                spawnPoint = {player1.position.x + player1.size.x, player1.position.y + player1.size.y / 2};
            } else {
                spawnPoint = {player1.position.x - 1, player1.position.y + player1.size.y / 2};
            }
                        Bullet* tempBullet = new Bullet(spawnPoint, {10, 5}, player1.direction, 10.0f);
            tempBullet->isActive = true;
            bullets.push_back(tempBullet);
        }

        CheckForCollisions(player1, obsticles, obstacleCount);
        CheckBulletCollision(bullets, enemies);
        for (Bullet* bullet : bullets)
        {
            bullet->update();
        }
        for(Enemy* enemy : enemies)
        {
            enemy->Update();
        }


    
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            player1.Draw();
            DrawBackground(obsticles, obstacleCount);
            DrawEnemies(enemies);
            DrawBullets(bullets);
    
        EndDrawing();
        //----------------------------------------------------------------------------------


        //Garbage Collection
        //----------------------------------------------------------------------------------
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet* b) {
            return !b->isActive;
        }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy* e) {
            return !e->isActive;
        }), enemies.end());
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


