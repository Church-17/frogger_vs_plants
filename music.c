// Include libs
#include "music.h"

void play_sound(int sound_id) {
    static Mix_Chunk* select;
    switch(sound_id) {
        case _LOAD_SOUNDS:
            select = Mix_LoadWAV("./audio/select.mp3");
            break;

        case MENU_SELECTION:
            Mix_PlayChannel(-1, select, 0);
            break;

        case _FREE_SOUNDS:
            Mix_FreeChunk(select);
            break;
    }
}