#pragma once

#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
#include <dc/sound/stream.h>
#include <wav/sndwav.h>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void PlayJumpSound();
    void PlayLaser1Sound();
    void PlayLaser2Sound();
    void PlayHitSound();
    void PlayMusic(int music);

private:
    sfxhnd_t jump;
    sfxhnd_t laser1;
    sfxhnd_t laser2;
    sfxhnd_t hit;

    snd_stream_hnd_t  main;
    snd_stream_hnd_t  menu;
    snd_stream_hnd_t  boss;
    snd_stream_hnd_t  end;
    snd_stream_hnd_t  currentTrack;
};