#include "raylib.h"
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void DrawBackground()
{
    // Draw rectangualar obstacles in the sky
    DrawRectangle(500, 350, 100, 50, DARKGRAY);
    DrawRectangle(300, 350, 150, 50, DARKGRAY);
    DrawRectangle(100, 300, 100, 50, DARKGRAY);
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

void playerCollision(Rectangle playerRect, Rectangle obstacleRect)
{
    if (CheckCollisionRecsDirection(playerRect, obstacleRect) != NONE)
    {
        cout << "Collision detected!" << endl;
    }
}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    // World constants
    int gravity = 2.0f;
    float jumpVelocity = -30.0f;
    bool isJumping = false;
    bool topCollision = false;

    int playerSpeed = 5.0f;
    float playerVelocityY = 0.0f;
    Vector2 playerPosition = { 0, (float)screenHeight};
    Vector2 playerSize = { 80, 80 };

    InitWindow(screenWidth, screenHeight, "MegaMan Example");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerPosition.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A)) playerPosition.x -= playerSpeed;


        // Collision detection with screen edges
        if (playerPosition.x < 0) playerPosition.x = 0;
        if (playerPosition.x > screenWidth - playerSize.x) playerPosition.x = screenWidth - playerSize.x;
        if (playerPosition.y < 0) playerPosition.y = 0;
        if (playerPosition.y > screenHeight - playerSize.y) playerPosition.y = screenHeight - playerSize.y;
        if (playerPosition.y + playerSize.y >= screenHeight)
        {
            playerPosition.y = screenHeight - playerSize.y;
            isJumping = false; // Reset jumping state when player hits the ground
            playerVelocityY = 0.0f;
        }

        // Define player rectangle
        Rectangle playerRect = { playerPosition.x, playerPosition.y, playerSize.x, playerSize.y };

        // Define obstacle rectangles
        Rectangle obstacle1 = { 500, 350, 100, 50 };
        Rectangle obstacle2 = { 300, 350, 150, 50 };
        Rectangle obstacle3 = { 100, 300, 100, 50 };

        // Check for collisions with obstacles
        CollisionDirection collision = NONE; 
        if(CheckCollisionRecsDirection(playerRect, obstacle1) != NONE)
        {
            collision = CheckCollisionRecsDirection(playerRect, obstacle1);
            if (collision == 1)
            {
                playerPosition.y = obstacle1.y - playerSize.y;
                isJumping = false;
                playerVelocityY = 0.0f;
                cout << "Collision detected on Bottom" << " RECT 1" << endl;
            }else if (collision == 2)
            {
                cout << "Collision detected on Top" << " RECT 1" << endl;
                isJumping = false;
                playerVelocityY = 0.0f;
                topCollision = true;
            }else if (collision == 3){
                playerPosition.x = obstacle1.x - playerSize.x;
                cout << "Collision detected on right" << " RECT 1" << endl;
            }else if (collision == 4)
            {
                playerPosition.x = obstacle1.x + obstacle1.width;
                cout << "Collision detected on left" << " RECT 1" << endl;      
            }
            // cout << "Collision detected!" << "RECT 1" << CheckCollisionRecsDirection(playerRect, obstacle1) << endl;
        }
        else if(CheckCollisionRecsDirection(playerRect, obstacle2) != NONE)
        {
            collision = CheckCollisionRecsDirection(playerRect, obstacle2);
            if (collision == 1)
            {
                playerPosition.y = obstacle2.y - playerSize.y;
                isJumping = false;
                playerVelocityY = 0.0f;
                cout << "Collision detected on Bottom" << " RECT 1" << endl;
            }else if (collision == 2)
            {
                cout << "Collision detected on Top" << " RECT 1" << endl;
                isJumping = false;
                playerVelocityY = 0.0f;
                topCollision = true;
            }else if (collision == 3){
                playerPosition.x = obstacle2.x - playerSize.x;
                cout << "Collision detected on right" << " RECT 1" << endl;
            }else if (collision == 4)
            {
                playerPosition.x = obstacle2.x + obstacle2.width;
                cout << "Collision detected on left" << " RECT 1" << endl;      
            }
        // cout << "Collision detected!" << "RECT 2" << CheckCollisionRecsDirection(playerRect, obstacle1) << endl;
        }
        else if(CheckCollisionRecsDirection(playerRect, obstacle3) != NONE)
        {
            collision = CheckCollisionRecsDirection(playerRect, obstacle3);
            if (collision == 1)
            {
                playerPosition.y = obstacle3.y - playerSize.y;
                isJumping = false;
                playerVelocityY = 0.0f;
                cout << "Collision detected on Bottom" << " RECT 1" << endl;
            }else if (collision == 2)
            {
                cout << "Collision detected on Top" << " RECT 1" << endl;
                isJumping = false;
                playerVelocityY = 0.0f;
                topCollision = true;
            }else if (collision == 3){
                playerPosition.x = obstacle3.x - playerSize.x;
                cout << "Collision detected on right" << " RECT 1" << endl;
            }else if (collision == 4)
            {
                playerPosition.x = obstacle3.x + obstacle3.width;
                cout << "Collision detected on left" << " RECT 1" << endl;      
            }
            // cout << "Collision detected!"<< "RECT 3" << CheckCollisionRecsDirection(playerRect, obstacle3) << endl;
        }



        // Jumping
        if (IsKeyPressed(KEY_SPACE) && !isJumping)
        {
            isJumping = true;
            playerVelocityY = jumpVelocity;
        }

        // Gravity

        playerPosition.y += gravity;
        playerVelocityY += gravity;
        playerPosition.y += playerVelocityY;

        
        
        

        topCollision = false;

        // cout << "Player Position: " << playerPosition.x << ", " << playerPosition.y + playerSize.y << endl;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawRectangle(playerPosition.x, playerPosition.y, 80, 80, MAROON);
            DrawText("Hello world!!", 0, 0, 20, LIGHTGRAY);
            DrawBackground();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


