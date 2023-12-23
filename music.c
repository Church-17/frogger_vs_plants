// Include libs
#include "music.h"
#include "utils.h"

// Define constant
#define MUSIC_FREQUENCY 48000
#define MUSIC_STEREO 2
#define DIM_MUSIC_BUFFER 256
#define N_CHANNELS 8
#define FIRST_FREE_CHANNEL (-1)
#define LOAD_SOUNDS (-1)
#define FREE_SOUNDS (-2)

void init_music(void) {
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        quit(ERR_INIT_MUSIC);
    }
    if(Mix_Init(MIX_INIT_MP3) == 0) {
        quit(ERR_INIT_MUSIC);
    }
    if(Mix_OpenAudio(MUSIC_FREQUENCY, MIX_DEFAULT_FORMAT, MUSIC_STEREO, DIM_MUSIC_BUFFER) < 0) {
        quit(ERR_INIT_MUSIC);
    }
    if(Mix_AllocateChannels(N_CHANNELS) != N_CHANNELS) {
        quit(ERR_INIT_MUSIC);
    }
    play_sound(LOAD_SOUNDS);
}

void play_sound(int sound_id) {
    static Mix_Chunk* select;
    switch(sound_id) {
        case LOAD_SOUNDS:
            select = Mix_LoadWAV("./audio/select.mp3");
            break;

        case MENU_SELECTION:
            Mix_PlayChannel(FIRST_FREE_CHANNEL, select, 0);
            break;

        case FREE_SOUNDS:
            Mix_FreeChunk(select);
            break;
    }
}

void free_music(void) {
    play_sound(FREE_SOUNDS);
    Mix_Quit();
	SDL_Quit();
}