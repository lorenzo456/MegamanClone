#include "player.h"
#include <ostream>
#include <iostream>
#include "bullet.h"
#include "soundManager.h"
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

Player::Player(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity, std::vector<Bullet*> &bullets, SoundManager* soundManager)
         : position(position), size(size), speed(speed), gravity(gravity), jumpVelocity(jumpVelocity), isJumping(false), playerVelocityY(0.0f), health(3), direction(1), bullets(bullets), isAlive(true)
         {
            hearts = LoadTexture("/rd/assets/Sprites/Hearts/HealthUI.png");
            hearts.width *= 5;
            hearts.height *= 5;
            heartsFrameWidth = (float)(hearts.width/1);   // Sprite one frame rectangle width
            heartsFrameHeight = (float)(hearts.height/7);           // Sprite one frame rectangle height
            
            fullHeart = LoadTexture("/rd/assets/Sprites/VirtualGuy/FullHeart.png");
            characterIdle = LoadTexture("/rd/assets/Sprites/VirtualGuy/Idle0.png");
            characterWalk = LoadTexture("/rd/assets/Sprites/VirtualGuy/Run(32x32).png");
            characterShoot = LoadTexture("/rd/assets/Sprites/VirtualGuy/Shoot(32x32).png");
            emptyHeart = LoadTexture("/rd/assets/Sprites/VirtualGuy/EmptyHeart.png");
            // laser = LoadSound("../Sounds/Laser/laser1.wav");
            // jumpSound = LoadSound("../Sounds/Jump/jump.wav");
            // hitSound = LoadSound("../Sounds/Hit/hit.wav");
            frameWidth = (float)(characterIdle.width/11);   // Sprite one frame rectangle width
            frameHeight = (float)(characterIdle.height/1);           // Sprite one frame rectangle height

            for(int i = 0; i < 11; i++) {
                idleFrames[i] = LoadTexture(TextFormat("/rd/assets/Sprites/VirtualGuy/Idle%d.png", i));
                walkFrames[i] = LoadTexture(TextFormat("/rd/assets/Sprites/VirtualGuy/Walk%d.png", i));
            }

            currentFrame = 0;
            currentLine = 0;
            frameCounter = 0;
            frameRec = { 0, 0, frameWidth, frameHeight };
            deathCount = 3;
            shootingTimer = 0;
            currentDeathCount =0;
            this->soundManager = soundManager;
            Player::Init(position, size, speed, gravity, jumpVelocity, health, direction,  bullets, isAlive);
         }
         

void Player::Init(Vector2 position, Vector2 size, float speed, float gravity, float jumpVelocity, int health, int direction, std::vector<Bullet*> &bullets, bool isAlive)
{
    this->position = position;
    this->size = size;
    this->speed = speed;
    this->gravity = gravity;
    this->jumpVelocity = jumpVelocity;
    this->bullets = bullets;
    this->currentFrame = 0;
    this->currentLine = 0;
    this->frameCounter = 0;
    this->frameRec = { 0, 0, frameWidth, frameHeight };
    this->deathCount = 3;
    this->isAlive = isAlive;
    this->direction = direction;
    this->health = health;
    this->shootingTimer = 0;
    invinsibleFrames = 60;
    currentInvinsibleFrame = 0;
    invinsible = false;
    currentDeathCount = 0;
}

void Player::Update() {
    
    if(health < 0){
        currentDeathCount++;
        if(currentDeathCount > deathCount)
        {
            isAlive = false;
        }
        return;
    }

    currentSpeed = 0;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || 
        IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||  // D-pad Right
        GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.3f)     // Analog stick right
    {
        position.x += speed;
        currentSpeed = speed;
        direction = 1;
    }
    
    // Movement - Left
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ||
        IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||   // D-pad Left
        GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.3f)    // Analog stick left
    {
        position.x -= speed; 
        currentSpeed = speed;
        direction = -1;
    }

    // Jumping
    if ((IsKeyPressed(KEY_SPACE) || 
         IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) &&  // A button
        !isJumping) 
    {
        isJumping = true;
        playerVelocityY = jumpVelocity;
        // PlaySound(jumpSound);
        soundManager->PlayJumpSound(); 
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


if (IsKeyPressed(KEY_E) || 
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))  // B button for shooting
    {
        isShooting = true;
        Vector2 spawnPoint;
        if (direction == 1) {
            spawnPoint = {position.x + size.x, position.y + size.y / 2};
        } else {
            spawnPoint = {position.x - 10, position.y + size.y / 2};
        }
        Bullet* tempBullet = new Bullet(spawnPoint, {10, 5}, direction, 10.0f);
        tempBullet->isActive = true;
        bullets.push_back(tempBullet);
        // PlaySound(laser);
        soundManager->PlayLaser1Sound();
    }

    if (invinsible){
        currentInvinsibleFrame++;
        if(currentInvinsibleFrame > invinsibleFrames)
        {
            invinsible = false;
            currentInvinsibleFrame = 0;
        }
    }


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

    if (isShooting) {
        shootingTimer += GetFrameTime();
        if (shootingTimer >= 0.1f) {
            isShooting = false;
            shootingTimer = 0;
        }
    }

}

void Player::OnHit(int damage){
    if(!isHit && !invinsible){
        isHit = true;
        health -= damage;
        invinsible = true;
        // PlaySound(hitSound);
        soundManager->PlayHitSound();
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
 if (health >= 0) {
        if (!invinsible || (currentInvinsibleFrame / 5) % 2 == 0) { 
            if(isShooting){
                    Rectangle sourceRec = { 0, 0, (float)characterShoot.width * direction, (float)characterShoot.height };
                    Rectangle destRec = { position.x, position.y, size.x, size.y };
                    DrawTexturePro(characterShoot, sourceRec, destRec, {0,0}, 0.0f, WHITE);
            }else{
                if (currentSpeed > 0) {
                    Rectangle sourceRec = { 0, 0, (float)walkFrames[currentFrame].width * direction, (float)walkFrames[currentFrame].height };
                    Rectangle destRec = { position.x, position.y, size.x, size.y };
                    DrawTexturePro(walkFrames[currentFrame], sourceRec, destRec, {0,0}, 0.0f, WHITE);
                } else {

                    Rectangle sourceRec = { 0, 0, (float)idleFrames[currentFrame].width * direction, (float)idleFrames[currentFrame].height };
                    Rectangle destRec = { position.x, position.y, size.x, size.y };
                    DrawTexturePro(idleFrames[currentFrame], sourceRec, destRec, {0,0}, 0.0f, WHITE);

                }
            }
        }
    }
    
    if(health == 3)
    {
        DrawTexture(fullHeart, 5, 5, WHITE);    
        DrawTexture(fullHeart, 37, 5, WHITE);   
        DrawTexture(fullHeart, 69, 5, WHITE);
    }else if (health == 2)
    {
        DrawTexture(fullHeart, 5, 5, WHITE);    
        DrawTexture(fullHeart, 37, 5, WHITE);   
        DrawTexture(emptyHeart, 69, 5, WHITE);
        // DrawTexture(heartsFrames[1], 0,0, WHITE);
    }else if (health == 1)
    {
        // DrawTexture(heartsFrames[2], 0,0, WHITE);
        DrawTexture(fullHeart, 5, 5, WHITE);    
        DrawTexture(emptyHeart, 37, 5, WHITE);   
        DrawTexture(emptyHeart, 69, 5, WHITE);
    }else{
        // DrawTexture(heartsFrames[3], 0,0, WHITE);
        DrawTexture(emptyHeart, 5, 5, WHITE);    
        DrawTexture(emptyHeart, 37, 5, WHITE);   
        DrawTexture(emptyHeart, 69, 5, WHITE);
    }
    
    

}



Rectangle Player::GetRectangle(){
    return { position.x, position.y, size.x, size.y };
}