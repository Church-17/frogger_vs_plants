// libsdl2-mixer-dev
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>
#include <stdlib.h>

//The music that will be played
// Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *scratch = NULL;

void play_sound(Mix_Chunk* sound);

int main(void) {
    // Init SDL engine
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) == -1) {
        return 1;    
    }
    
    //Init SDL_mixer
    if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        return 1;    
    }
    
    //Load the music
    // music = Mix_LoadMUS( "beat.wav" );
    
    // Load effects
    scratch = Mix_LoadWAV( "../audio/select_new.mp3" );

    char str[100];
    while(str[0] != 'q') {
        printf("ready\n");
        scanf("%s", str);
        getchar();
        play_sound(scratch);
    }

    // Free effects
    Mix_FreeChunk( scratch );
    scratch = NULL;
    
    //Free the music
    // Mix_FreeMusic( music );
    
    //Quit SDL_mixer
    Mix_Quit();
    
    //Quit SDL
    SDL_Quit();

}

void play_sound(Mix_Chunk* sound) {

    Mix_PlayChannel(-1, sound, 0);

}