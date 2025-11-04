#include "raylib.h"
#include <iostream>
#include <algorithm>
#include "player.h"
#include "obstacle.h"
#include "bullet.h"
#include "enemy.h"
#include "boss.h"
#include <cmath>
#include "soundManager.h"
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
#include <wav/sndwav.h>
using namespace std;

// Scaling constants from 800x450 to 640x480
const float SCALE_X = 0.8f;  // 640/800
const float SCALE_Y = 1.067f; // 480/450

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

void UpdateBackgroundFrameRec(Rectangle &frameRec, int &frameCounter, int &currentFrame, float frameWidth, float frameHeight, int currentLine)
{
    frameCounter++;
    if (frameCounter >= 10)
    {
        currentFrame++;
        if (currentFrame >= 3)
        {
            currentFrame = 0;
        }
        frameCounter = 0;
    }
    frameRec.x = frameWidth * currentFrame;
    frameRec.y = frameHeight * currentLine;
}

void DrawGameBackground(Texture2D* backgroundFrames, int currentFrame)
{
    DrawTexture(backgroundFrames[currentFrame], 50, 0, WHITE);
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

void CheckPlayerBossCollision(Player& player, Boss& boss)
{
    if (CheckCollisionRecsDirection(player.GetRectangle(), boss.GetRectangle()) != NONE)
    {
        CollisionDirection collision = CheckCollisionRecsDirection(player.GetRectangle(), boss.GetRectangle());
        if (collision != NONE && boss.isActive)
        {
            player.OnHit(1);
            player.position.x += player.size.x * player.direction * -1;
        }
    }
}

void CheckBulletBossCollision(std::vector<Bullet*>& bullets, Boss& boss)
{
    for (Bullet* bullet : bullets)
    {
        if (CheckCollisionRecsDirection(bullet->GetRectangle(), boss.GetRectangle()) != NONE)
        {
            CollisionDirection collision = CheckCollisionRecsDirection(bullet->GetRectangle(), boss.GetRectangle());
            if (collision != NONE && boss.isInvulnerable == false && boss.isActive)
            {
                bullet->isActive = false;
                boss.isHit = true;
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
            enemy->direction = 1;
        }
        else
        {
            enemy->direction = -1;
        }

        if (CheckCollisionRecsDirection(player.GetRectangle(), enemy->GetRectangle()) && !player.invinsible)
        {
            player.OnHit(1);
            player.position.x += player.size.x * player.direction * -1;
        }
    }
}

void init_boss_level(std::vector<Obstacle*> &obstacles)
{
    // Scaled for 640x480
    new Obstacle({0, 448},   {640, 40}, obstacles);  // Floor
    new Obstacle({0, 320},   {160, 40}, obstacles);  // Left lower platform
    new Obstacle({64, 192}, {160, 40}, obstacles);   // Mid-left platform
    new Obstacle({240, 96}, {160, 40}, obstacles);   // High-up left platform
    new Obstacle({276, 267}, {80, 40}, obstacles);   // Center platform
    new Obstacle({640 - 152, 352}, {160, 40}, obstacles);  // Right lower platform
    new Obstacle({640 - 72 - 160, 224}, {160, 40}, obstacles);  // Mid-right platform
}

void init_level_1(std::vector<Obstacle*> &obstacles, std::vector<Enemy*>&enemies, std::vector<Bullet*>&bullets, SoundManager* soundManager)
{
    // Scaled for 640x480
    new Obstacle({0, 448},   {640, 40}, obstacles);
    new Obstacle({0, 320},   {160, 40}, obstacles);
    new Obstacle({64, 192}, {160, 40}, obstacles);
    new Obstacle({240, 96}, {160, 40}, obstacles);
    new Obstacle({276, 267}, {80, 40}, obstacles);
    new Obstacle({640 - 152, 352}, {160, 40}, obstacles);
    new Obstacle({640 - 72 - 160, 224}, {160, 40}, obstacles);

    new Enemy({80, 288}, {32, 32}, 5.0f, 0, bullets, 2.0f, enemies, soundManager);
    new Enemy({560, 320}, {32, 32}, 0, 0, bullets, 2.0f, enemies, soundManager);
    new Enemy({160, 160}, {32, 32}, 3.5f, 0, bullets, 2.0f, enemies, soundManager);
    new Enemy({440, 192}, {32, 32}, 4.0f, 0, bullets, 2.0f, enemies, soundManager);
    new Enemy({320, 64}, {32, 32}, 3.0f, 0, bullets, 1.0f, enemies, soundManager);
}

void init_level_2(std::vector<Obstacle*> &obstacles, std::vector<Enemy*>&enemies, std::vector<Bullet*>&bullets, Player &player, SoundManager* soundManager)
{
    // Scaled for 640x480
    new Obstacle({0, 440},   {640, 40}, obstacles);
    new Obstacle({0, 320},   {160, 40}, obstacles);
    new Obstacle({64, 192}, {160, 40}, obstacles);
    new Obstacle({240, 96}, {160, 40}, obstacles);
    new Obstacle({276, 267}, {80, 40}, obstacles);
    new Obstacle({640 - 152, 352}, {160, 40}, obstacles);
    new Obstacle({640 - 72 - 160, 224}, {160, 40}, obstacles);

    new Enemy({640, 440}, {32, 32}, 180.0f, 1, bullets, 2.0f, enemies, soundManager);
    new Enemy({0, 440}, {32, 32}, 180.0f, 1, bullets, 2.0f, enemies, soundManager);
    new Enemy({560, 192}, {32, 32}, 180.0f, 1, bullets, 2.0f, enemies, soundManager);
    new Enemy({80, 192}, {32, 32}, 180.0f, 1, bullets, 2.0f, enemies, soundManager);
    new Enemy({320, 64}, {32, 32}, 3.0f, 1, bullets, 1.0f, enemies, soundManager);
}

float Distance(const Vector2& a, const Vector2& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

void MoveEnemiesToPlayerIfClose(Player& player, std::vector<Enemy*>& enemies, float distanceThreshold)
{
    if(!player.invinsible){
        for (Enemy* enemy : enemies)
        {
            if (enemy->enemyType == 1)
            {
                float distancexy = Distance(player.position, enemy->position);
                float distancex = std::abs(player.position.x - enemy->position.x);
                if (distancexy < distanceThreshold && distancex > 2)
                {
                    enemy->FollowPlayer(player.position);
                }
            }
        }
    }
}

void UpdateHomeScreen(GameState& gameState, const char* titleText, const char* startText, int screenWidth, int screenHeight, int titleWidth, int startWidth, bool &firstFrameHomeScreen)
{

    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) || IsKeyPressed(KEY_ENTER))
    {
        gameState = GAME;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(titleText, (screenWidth / 2) - (titleWidth / 2), screenHeight / 2 - 20, 20, LIGHTGRAY);
    DrawText(startText, (screenWidth / 2) - (startWidth / 2), screenHeight / 2 + 10, 20, LIGHTGRAY);
    EndDrawing();
}

void UpdateEndScreen(GameState& gameState, const char* titleText, const char* startText, int screenWidth, int screenHeight, int titleWidth, int startWidth, bool &firstEndFrame)
{
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) || IsKeyPressed(KEY_ENTER))
    {
        gameState = START;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(titleText, (screenWidth / 2) - (titleWidth / 2), screenHeight / 2 - 20, 20, LIGHTGRAY);
    DrawText(startText, (screenWidth / 2) - (startWidth / 2), screenHeight / 2 + 10, 20, LIGHTGRAY);
    EndDrawing();
}

void UpdateGameOverScreen(GameState& gameState, const char* titleText, const char* startText, int screenWidth, int screenHeight, int titleWidth, int startWidth, bool &firstGameOverFrame)
{
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) || IsKeyPressed(KEY_ENTER))
    {
        gameState = START;
    }
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(titleText, (screenWidth / 2) - (titleWidth / 2), screenHeight / 2 - 20, 20, LIGHTGRAY);
    DrawText(startText, (screenWidth / 2) - (startWidth / 2), screenHeight / 2 + 10, 20, LIGHTGRAY);
    EndDrawing();
}

void Draw(Player player1, std::vector<Obstacle*> &obstacles, std::vector<Enemy*> &enemies, std::vector<Bullet*> &bullets, Boss &boss1, Texture2D* background, int currentFrame)
{
    BeginDrawing();
        ClearBackground(BLACK);
        DrawGameBackground(background, currentFrame);
        player1.Draw();
        DrawBackground(obstacles);
        DrawEnemies(enemies);
        DrawBullets(bullets);
        if(boss1.isActive && boss1.initialized){
            boss1.Draw();
        }
    EndDrawing();
}

void GarbageCollection(std::vector<Enemy*> &enemies,std::vector<Bullet*> &bullets,bool cleanAll = false)
{
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
    
    // Initialization - Dreamcast resolution
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "MegaMan Dreamcast");

        // Only need this for sound effects
    snd_mem_init(1024 * 1024 * 2);  // Allocate 2MB for sound samples

    const char* titleText = "Welcome to MegaMan!";
    const char* startText = "Press START to Begin";
    const char* gameoverText = "Game Over";
    const char* restartText = "Press START to Restart";
    const char* endText = "Congratulations! You Win!";
    const char* playAgainText = "Press START to Play Again";
    
    int endTextWidth = MeasureText(endText, 20);
    int playAgainWidth = MeasureText(playAgainText, 20);
    int titleWidth = MeasureText(titleText, 20);
    int startWidth = MeasureText(startText, 20);
    int gameoverWidth = MeasureText(gameoverText, 20);
    int restartWidth = MeasureText(restartText, 20);

    std::vector<Obstacle*> obstacles;
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    
    // Texture2D background = LoadTexture("/rd/assets/Sprites/Background/background.png");
    // float frameWidth = (float)(background.width/5);
    // float frameHeight = (float)(background.height/1);
    int currentFrame = 0;
    // int currentLine = 0;
    int frameCounter = 0;
    // Rectangle frameRec = { 0, 0, frameWidth, frameHeight };
    Texture2D backgroundFrames[5];

    for(int i = 0; i < 5; i++) {
        backgroundFrames[i] = LoadTexture(TextFormat("/rd/assets/Sprites/BackgroundS/b%d.png", i));
    }

    // backgroundFrames[0] = LoadTexture(TextFormat("/rd/assets/Sprites/BackgroundS/b0.png"));
    
    snd_init();
    snd_stream_init();
    wav_init();

    SoundManager soundManager = SoundManager();
    soundManager.PlayMusic(0); 
    Player player1({40, screenHeight}, {32, 32}, 5.0f, 1.5f, -20.0f, bullets, &soundManager);

    Boss boss1({480, 53}, {80, 80}, bullets, &soundManager);
    boss1.isActive = false;
    boss1.initialized = false;

    bool has_init_level1 = false;
    bool has_init_level2 = false;
    bool has_init_level3 = false;
    bool firstGameOverFrame = false;
    bool firstFrameHomeScreen = false;
    bool firstEndFrame = false;
    
    GameState gameState = START;
    int currentLevel = 1;


    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {

        if(gameState == GAME)
        {

            if(!has_init_level1 && currentLevel == 1)
            {
                firstGameOverFrame = false;
                firstFrameHomeScreen = false;
                init_level_1(obstacles, enemies, bullets, &soundManager);
                player1.Init({40, screenHeight}, {32, 32}, 5.0f, 1.5f, -20.0f, 3, 1, bullets, true);
                has_init_level1 = true;
                has_init_level2 = false;
                soundManager.PlayMusic(1);
            }           
            else if(!has_init_level2 && currentLevel == 2)
            {
                GarbageCollection(enemies, bullets, true);
                init_level_2(obstacles, enemies, bullets, player1, &soundManager);
                has_init_level2 = true;
            }
            else if(currentLevel > 2)
            {
                if(!has_init_level3)
                {
                    GarbageCollection(enemies, bullets, true);
                    obstacles.clear();
                    init_boss_level(obstacles);
                    has_init_level3 = true;
                    boss1.Init();
                    soundManager.PlayMusic(3);
                }
            }

            CheckPlayerEnemyProximity(player1, enemies);
            CheckForCollisions(player1, obstacles);
            CheckBulletCollision(bullets, enemies);
            CheckBulletPlayerCollision(bullets, &player1);
            CheckObstacleEnemyCollision(obstacles, enemies);
            CheckBulletBossCollision(bullets, boss1);
            CheckPlayerBossCollision(player1, boss1);

            player1.Update();
            if (!player1.isAlive)
            {
                gameState = GAMEOVER;
            }

            UpdateBullets(bullets);
            UpdateEnemies(enemies);
            MoveEnemiesToPlayerIfClose(player1, enemies, 200);
            if(has_init_level3){
                boss1.Update();
            }

            if (enemies.empty() && !boss1.initialized || 
                std::all_of(enemies.begin(), enemies.end(), [](Enemy* e) { return !e->isActive; }) && !boss1.initialized || 
                boss1.initialized && boss1.health <= 0 && boss1.isActive == false) 
            {
                currentLevel++;
                if (currentLevel > 3)
                {
                    gameState = END;
                }
            }

            frameCounter++;
            if (frameCounter >= 10)
            {
                currentFrame++;
                if (currentFrame >= 3)
                {
                    currentFrame = 0;
                }
                frameCounter = 0;
            }

            Draw(player1, obstacles, enemies, bullets, boss1, backgroundFrames, currentFrame);
            GarbageCollection(enemies, bullets);
        }
        else if (gameState == START)
        {
            UpdateHomeScreen(gameState, titleText, startText, screenWidth, screenHeight, titleWidth, startWidth, firstFrameHomeScreen);
        }
        else if(gameState == GAMEOVER)
        {
            GarbageCollection(enemies, bullets, true);
            UpdateGameOverScreen(gameState, gameoverText, restartText, screenWidth, screenHeight, gameoverWidth, restartWidth, firstGameOverFrame);
            has_init_level1 = false;
            currentLevel = 1;
            boss1.Init(true);
            has_init_level3 = false;
            has_init_level1 = false;
            has_init_level2 = false;
            soundManager.PlayMusic(4);
        }
        else if (gameState == END)
        {
            has_init_level1 = false;
            GarbageCollection(enemies, bullets, true);
            UpdateEndScreen(gameState, endText, playAgainText, screenWidth, screenHeight, endTextWidth, playAgainWidth, firstEndFrame);
            currentLevel = 1;
            boss1.Init(true);
            has_init_level3 = false;
            has_init_level1 = false;
            has_init_level2 = false;
            soundManager.PlayMusic(4);
        }
    }

    CloseWindow();

    // Shutdown sound system before exiting
    snd_mem_shutdown();
    return 0;
}