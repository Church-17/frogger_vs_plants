// Include libs
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Define constant
#define MUSIC_FREQUENCY 48000
#define N_MUSIC_CHANNELS 2
#define DIM_MUSIC_BUFFER 256

// Define sounds
#define _LOAD_SOUNDS (-1)
#define _FREE_SOUNDS (-2)
#define MENU_SELECTION 1

void play_sound(int sound_id);