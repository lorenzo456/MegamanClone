#include "soundManager.h"
#include <dc/sound/stream.h>

SoundManager::SoundManager() 
{
    jump = snd_sfx_load("/rd/assets/sound/jumpOG.wav");
    laser1 = snd_sfx_load("/rd/assets/sound/laser1.wav");
    laser2 = snd_sfx_load("/rd/assets/sound/laser2.wav");
    hit = snd_sfx_load("/rd/assets/sound/hit.wav");

    menu = wav_create("/rd/assets/Music/main.wav", 1);
    main = wav_create("/rd/assets/Music/game.wav", 1);
    boss = wav_create("/rd/assets/Music/boss.wav", 1);
    end = wav_create("/rd/assets/Music/end.wav", 1);
}

SoundManager::~SoundManager() {
    snd_sfx_unload(jump);
    snd_sfx_unload(laser1);
    snd_sfx_unload(laser2);
    snd_sfx_unload(hit);
}

void SoundManager::PlayJumpSound() {
    if (jump != SFXHND_INVALID) {
        snd_sfx_play(jump, 255, 128);
    }
}

void SoundManager::PlayHitSound() {
    if(hit != SFXHND_INVALID) {
        snd_sfx_play(hit, 255, 128);
    }
}
void SoundManager::PlayLaser1Sound() {
    if(laser1 != SFXHND_INVALID) {
        snd_sfx_play(laser1, 255, 128);
    }
}
void SoundManager::PlayLaser2Sound() {
    if(laser2 != SFXHND_INVALID) {
        snd_sfx_play(laser2, 255, 128);
    }
}

void SoundManager::PlayMusic(int music) 
{
    if (music == 0){
        music = menu;
    } else if (music == 1)
    {
        music = main;
    }else if (music == 3)
    {
        music = boss;
    }else if (music == 4)
    {
        music = end;
    }
    

    wav_stop(currentTrack);
    currentTrack = music;
    wav_volume(currentTrack, 255);
    wav_play(currentTrack);
}

// void SoundManager::LoadMusic() {
//     main = snd_stream_load("/rd/assets/Music/main.wav");
//     menu = snd_stream_load("/rd/assets/Music/menu.wav");
//     gameover = snd_stream_load("/rd/assets/Music/gameover.wav");
//     boss = snd_stream_load("/rd/assets/Music/boss.wav");
// }