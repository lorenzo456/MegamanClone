#include "player.h"


Player::Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity)
         : position(position), size(size), speed(speed), gravity(gravity), jumpVelocity(jumpVelocity), isJumping(false), playerVelocityY(0.0f), direction(1){}

void Player::Update() {
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

}

void Player::Draw() {
    DrawRectangle(position.x, position.y, size.x, size.y, MAROON);
}

Rectangle Player::GetRectangle(){
    return { position.x, position.y, size.x, size.y };
}