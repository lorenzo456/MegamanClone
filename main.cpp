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

void UpdateEnemies(std::vector<Enemy*>& enemies)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update();
    }
}

void UpdateBullets(std::vector<Bullet*>& bullets)
{
    for (Bullet* bullet : bullets)
    {
        bullet->update();
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

void CheckObstacleEnemyCollision(std::vector<Obstacle*>& obstacles, std::vector<Enemy*>& enemies)
{
    for (Obstacle* obstacle : obstacles)
    {
        for (Enemy* enemy : enemies)
        {
            if (CheckCollisionRecsDirection(obstacle->GetRectangle(), enemy->GetRectangle()) != NONE)
            {
            CollisionDirection collision = CheckCollisionRecsDirection(obstacle->GetRectangle(), enemy->GetRectangle());
            if (collision == BOTTOM)
            {
                enemy->position.y = obstacle->position.y - enemy->size.y;
                enemy->velocityY = 0.0f;
            }
            else if (collision == LEFT)
            {
                enemy->position.x = obstacle->position.x - enemy->size.x;
            }
            else if (collision == RIGHT)
            {
                enemy->position.x = obstacle->position.x + obstacle->size.x;
            }
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

        if (CheckCollisionRecsDirection(player.GetRectangle(), enemy->GetRectangle()))
        {
            player.OnHit(1);
            player.position.x += player.size.x * player.direction * -1;
        }
    }
}


void init_level_1(std::vector<Obstacle*> &obstacles, std::vector<Enemy*>&enemies, std::vector<Bullet*>&bullets)
{
    new Obstacle({0, 420},   {800,40}, obstacles);  // Floor (Y = 420)
    new Obstacle({0, 300},   {200, 40}, obstacles);  // Left lower platform (Y = 330)
    new Obstacle({80, 180}, {200, 40}, obstacles); // Mid-left platform (Y = 210)
    new Obstacle({300, 90}, {200, 40}, obstacles);  // High-up left platform (Y = 110)
    new Obstacle({345, 250}, {100, 40}, obstacles);  // High-up left platform (Y = 110)
    new Obstacle({800 - 190, 330}, {200, 40}, obstacles);  // Mirrored right lower platform (Y = 330)
    new Obstacle({800 - 90 - 200, 210}, {200, 40}, obstacles);  // Mirrored mid-right platform (Y = 210)

    new Enemy({100, 300 - 32}, {32, 32}, 5.0f, 0, bullets, 2.0f, enemies);  // On left lower platform
    new Enemy({700, 330 - 32}, {32, 32}, 0, 0,   bullets, 2.0f, enemies);  // On mirrored right lower platform
    new Enemy({200, 180 - 32}, {32, 32}, 3.5f, 1, bullets, 2.0f, enemies);  // On mirrored mid-right platform
    new Enemy({550, 210 - 32}, {32, 32}, 4.0f, 0, bullets, 2.0f, enemies);  // On mid-left platform
    new Enemy({400, 90 -  32},  {32, 32}, 3.0f, 0, bullets, 1.0f, enemies);  // On high-up left platform
}

void init_level_2(std::vector<Obstacle*> &obstacles, std::vector<Enemy*>&enemies, std::vector<Bullet*>&bullets)
{
    new Obstacle({0, 420},   {800,40}, obstacles);  // Floor (Y = 420)
    new Obstacle({0, 300},   {200, 40}, obstacles);  // Left lower platform (Y = 330)
    new Obstacle({80, 180}, {200, 40}, obstacles); // Mid-left platform (Y = 210)
    new Obstacle({300, 90}, {200, 40}, obstacles);  // High-up left platform (Y = 110)
    new Obstacle({345, 250}, {100, 40}, obstacles);  // High-up left platform (Y = 110)
    new Obstacle({800 - 190, 330}, {200, 40}, obstacles);  // Mirrored right lower platform (Y = 330)
    new Obstacle({800 - 90 - 200, 210}, {200, 40}, obstacles);  // Mirrored mid-right platform (Y = 210)

    new Enemy({100, 300 - 52}, {32, 32}, 5.0f, 0, bullets, 2.0f, enemies);  // On left lower platform
    new Enemy({700, 330 - 52}, {32, 32}, 0, 0,   bullets, 2.0f, enemies);  // On mirrored right lower platform
    new Enemy({200, 180 - 52}, {32, 32}, 3.5f, 1, bullets, 2.0f, enemies);  // On mirrored mid-right platform
    new Enemy({550, 210 - 52}, {32, 32}, 4.0f, 0, bullets, 2.0f, enemies);  // On mid-left platform
    new Enemy({400, 90 -  52},  {32, 32}, 3.0f, 0, bullets, 1.0f, enemies);  // On high-up left platform
}



void UpdateHomeScreen(GameState& gameState, const char* titleText, const char* startText, int screenWidth, int screenHeight, int titleWidth, int startWidth)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        gameState = GAME;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(titleText, (screenWidth / 2) - (titleWidth / 2), screenHeight / 2 - 20, 20, LIGHTGRAY);
    DrawText(startText, (screenWidth / 2) - (startWidth / 2), screenHeight / 2 + 10, 20, LIGHTGRAY);
    EndDrawing();
}

void UpdateGameOverScreen(GameState& gameState, const char* titleText, const char* startText, int screenWidth, int screenHeight, int titleWidth, int startWidth)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        gameState = START;        
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(titleText, (screenWidth / 2) - (titleWidth / 2), screenHeight / 2 - 20, 20, LIGHTGRAY);
    DrawText(startText, (screenWidth / 2) - (startWidth / 2), screenHeight / 2 + 10, 20, LIGHTGRAY);
    EndDrawing();
}

void Draw(Player player1, std::vector<Obstacle*> &obstacles, std::vector<Enemy*> &enemies, std::vector<Bullet*> &bullets)
{
    
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
}

void GarbageCollection(std::vector<Enemy*> &enemies,std::vector<Bullet*> &bullets,bool cleanAll = false)
{
        //Garbage Collection
        //----------------------------------------------------------------------------------
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet* b) {
            return !b->isActive;
        }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy* e) {
            return !e->isActive;
        }), enemies.end());


        if (cleanAll) {
            for (Bullet* bullet : bullets) {
            delete bullet;
            }
            bullets.clear();

            for (Enemy* enemy : enemies) {
            delete enemy;
            }
            enemies.clear();
        }
}



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;


    InitWindow(screenWidth, screenHeight, "MegaMan Example");
    InitAudioDevice();
    const char* titleText = "Welcome to MegaMan!";
    const char* startText = "Press ENTER to Start";
    const char* gameoverText = "Game Over";
    const char* restartText = "Press ENTER to Restart";

    int titleWidth = MeasureText(titleText, 20);
    int startWidth = MeasureText(startText, 20);

    int gameoverWidth = MeasureText(gameoverText, 9);
    int restartWidth = MeasureText(restartText, 22);

    std::vector<Obstacle*> obstacles;
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;

    Player player1({50, screenHeight}, {32, 32}, 5.0f, 1.5f, -20.0f, bullets); // Adjusted size
    bool has_init_level1 = false;
    bool has_init_level2 = false;

    GameState gameState = START;
    int currentLevel = 1;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------


        if(gameState == GAME)
        {
            if(!has_init_level1 && currentLevel == 1)
            {
                init_level_1(obstacles, enemies, bullets);
                player1.Init({50, screenHeight}, {32, 32}, 5.0f, 1.5f, -20.0f, 3, 1, bullets, true);
                has_init_level1 = true;
            }else if(!has_init_level2 && currentLevel == 2)
            {
                GarbageCollection(enemies, bullets, true);
                init_level_2(obstacles, enemies, bullets);
                has_init_level2 = true;
            }
                //Check directions
                CheckPlayerEnemyProximity(player1, enemies);

                //Check colissions
                CheckForCollisions(player1, obstacles);
                CheckBulletCollision(bullets, enemies);
                CheckBulletPlayerCollision(bullets, &player1);
                CheckObstacleEnemyCollision(obstacles, enemies);


                //Update functions
                player1.Update();
                if (!player1.isAlive)
                {
                    gameState = GAMEOVER;
                }
                
                UpdateBullets(bullets);
                UpdateEnemies(enemies);

                if (enemies.empty() || std::all_of(enemies.begin(), enemies.end(), [](Enemy* e) { return !e->isActive; }))
                {
                    currentLevel++;
                }                
                

                Draw(player1, obstacles, enemies, bullets);
                GarbageCollection(enemies, bullets);

        }else if (gameState == START)
        {
            UpdateHomeScreen(gameState, titleText, startText, screenWidth, screenHeight, titleWidth, startWidth);
        }else if(gameState == GAMEOVER)
        {
            GarbageCollection(enemies, bullets, true);
            UpdateGameOverScreen(gameState, gameoverText, restartText, screenWidth, screenHeight, gameoverWidth, restartWidth);
            has_init_level1 = false;
            currentLevel = 1;
        }

    }



    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


