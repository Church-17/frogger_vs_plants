// Include libs
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Define sounds
#define MENU_SELECTION_SOUND 0
#define N_EFFECTS 1

void init_music(void);
void play_sound(int sound_id);
void free_music(void);