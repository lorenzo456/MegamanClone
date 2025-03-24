#include "player.h"
#include <ostream>
#include <iostream>


Player::Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity)
         : position(position), size(size), speed(speed), gravity(gravity), jumpVelocity(jumpVelocity), isJumping(false), playerVelocityY(0.0f), health(3), direction(1)
         {

            character = LoadTexture("Sprites/VirtualGuy/Idle(32x32).png");
            frameWidth = (float)(character.width/11);   // Sprite one frame rectangle width
            frameHeight = (float)(character.height/1);           // Sprite one frame rectangle height
            currentFrame = 0;
            currentLine = 0;
            frameCounter = 0;
            
            frameRec = { 0, 0, frameWidth, frameHeight };

         }
         

void Player::Update() {

    if(health < 0)
        return;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        position.x += speed;
        direction = 1;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) 
    {
        position.x -= speed; 
        direction = -1;
    }

    // Jumping
    if (IsKeyPressed(KEY_SPACE) && !isJumping) {
        isJumping = true;
        playerVelocityY = jumpVelocity;
    }

    // Apply gravity
    playerVelocityY += gravity;
    position.y += playerVelocityY;

    // Collision detection with screen edges
    if (position.x < 0) position.x = 0;
    if (position.x > GetScreenWidth() - size.x) position.x = GetScreenWidth() - size.x;
    if (position.y > GetScreenHeight() - size.y) {
        position.y = GetScreenHeight() - size.y;
        isJumping = false; // Reset jumping state when player hits the ground
        playerVelocityY = 0.0f;
    }
    
    // cout << "Player Position: " << playerPosition.x << ", " << playerPosition.y + playerSize.y << endl;


    frameCounter++;
    if (frameCounter > 2)
    {
        currentFrame++;

        if (currentFrame >= 11)
        {
            currentFrame = 0;
            currentLine++;

            if (currentLine >= 1)
            {
                currentLine = 0;
            }
        }

        frameCounter = 0;
    }
    frameRec.x = frameWidth*currentFrame;
    frameRec.y = frameHeight*currentLine;

}

void Player::OnHit(int damage){
    if(!isHit){
        isHit = true;
        health -= damage;
        if(health < 0){
            OnDeath();
        }
    }
    isHit = false;
}

void Player::OnDeath(){
    position.x = -5000;
    position.y = -5000;
}

void Player::Draw() {
    if(health >= 0)
    {
        // DrawRectangle(position.x, position.y, size.x, size.y, MAROON);
        DrawTextureRec(character, { frameRec.x, frameRec.y, frameRec.width * direction, frameRec.height }, position, WHITE);
    }
}

Rectangle Player::GetRectangle(){
    return { position.x, position.y, size.x, size.y };
}