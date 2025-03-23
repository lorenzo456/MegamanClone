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
void DrawBackground(std::vector<Obstacle*> &obstacles)
{
    for (Obstacle* obstacle : obstacles)
    {
        obstacle->Draw();
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

enum GameState {
    START,
    GAME,
    GAMEOVER,
    END,
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

void CheckForCollisions(Player& player, std::vector<Obstacle*> &obstacles)
{
    for (Obstacle* obstacle : obstacles)
    {
        if (CheckCollisionRecsDirection(player.GetRectangle(), obstacle->GetRectangle()) != NONE)
        {
            CollisionDirection collision = CheckCollisionRecsDirection(player.GetRectangle(), obstacle->GetRectangle());
            if (collision == 1)
            {
                player.position.y = obstacle->position.y - player.size.y;
                player.isJumping = false;
                player.playerVelocityY = 0.0f;
            }
            else if (collision == 2)
            {
                player.position.y = obstacle->position.y + obstacle->size.y;
                player.isJumping = false;
                player.playerVelocityY = 0.0f;
            }
            else if (collision == 3)
            {
                player.position.x = obstacle->position.x - player.size.x;
            }
            else if (collision == 4)
            {
                player.position.x = obstacle->position.x + obstacle->size.x;
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
void CheckBulletPlayerCollision(std::vector<Bullet*>& bullets, Player* player)
{
    for (Bullet* bullet : bullets)
    {
            if (CheckCollisionRecsDirection(bullet->GetRectangle(), player->GetRectangle()) != NONE)
            {
                CollisionDirection collision = CheckCollisionRecsDirection(bullet->GetRectangle(), player->GetRectangle());
                if (collision != NONE)
                {
                    bullet->isActive = false;
                    player->OnHit(1);
                }
            }
    }
}

bool IsPlayerToRightOfObstacle(Player& player, Enemy& enemy)
{
    return player.position.x > enemy.position.x + enemy.size.x;
}

void CheckPlayerEnemyProximity(Player& player, std::vector<Enemy*>& enemies)
{
    for (Enemy* enemy : enemies)
    {
        if (IsPlayerToRightOfObstacle(player, *enemy))
        {
            enemy->direction = 1; // Change direction to right
        }
        else
        {
            enemy->direction = -1; // Change direction to left
        }
    }
}


void init_level_1()
{

}


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    Player player1({50, screenHeight}, {40, 50}, 5.0f, 1.5f, -20.0f); // Adjusted size


    std::vector<Obstacle*> obstacles;
    Obstacle obstacle1({0, 420},   {800, 30});  // Floor (Y = 420)
    Obstacle obstacle2({0, 300},   {200, 30});  // Left lower platform (Y = 330)
    Obstacle obstacle3({80, 180}, {200, 30}); // Mid-left platform (Y = 210)
    Obstacle obstacle4({300, 90}, {200, 30});  // High-up left platform (Y = 110)
    Obstacle obstacle5({345, 250}, {100, 30});  // High-up left platform (Y = 110)
    Obstacle obstacle6({800 - 190, 330}, {200, 30});  // Mirrored right lower platform (Y = 330)
    Obstacle obstacle7({800 - 90 - 200, 210}, {200, 30});  // Mirrored mid-right platform (Y = 210)
    obstacles.push_back(&obstacle1);
    obstacles.push_back(&obstacle2);
    obstacles.push_back(&obstacle3);
    obstacles.push_back(&obstacle4);
    obstacles.push_back(&obstacle5);
    obstacles.push_back(&obstacle6);
    obstacles.push_back(&obstacle7);
    

    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;

    Enemy enemy1({100, 300 - 50}, {50, 50}, 5.0f, 0, bullets, 2.0f);  // On left lower platform
    Enemy enemy4({700, 330 - 50}, {50, 50}, 0, 0,   bullets, 2.0f);  // On mirrored right lower platform
    Enemy enemy5({200, 180 - 50}, {50, 50}, 3.5f, 1, bullets, 2.0f);  // On mirrored mid-right platform
    Enemy enemy2({550, 210 - 50}, {50, 50}, 4.0f, 0, bullets, 2.0f);  // On mid-left platform
    Enemy enemy3({400, 90 - 50}, {50, 50}, 3.0f, 0, bullets, 1.0f);  // On high-up left platform
    enemies.push_back(&enemy1);
    enemies.push_back(&enemy2);
    enemies.push_back(&enemy3);
    enemies.push_back(&enemy4);
    enemies.push_back(&enemy5);


    InitWindow(screenWidth, screenHeight, "MegaMan Example");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------


        //PlayerController
        player1.Update();
        if (IsKeyPressed(KEY_E))
        {
            Vector2 spawnPoint;
            if (player1.direction == 1) {
                spawnPoint = {player1.position.x + player1.size.x, player1.position.y + player1.size.y / 2};
            } else {
                spawnPoint = {player1.position.x - 10, player1.position.y + player1.size.y / 2};
            }
            Bullet* tempBullet = new Bullet(spawnPoint, {10, 5}, player1.direction, 10.0f);
            tempBullet->isActive = true;
            bullets.push_back(tempBullet);
        }

        //Check directions
        CheckPlayerEnemyProximity(player1, enemies);

        //Check colissions
        CheckForCollisions(player1, obstacles);
        CheckBulletCollision(bullets, enemies);
        CheckBulletPlayerCollision(bullets, &player1);

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
            DrawBackground(obstacles);
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


