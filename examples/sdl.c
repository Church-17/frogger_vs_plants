// libsdl2-mixer-dev
#include <SDL2/SDL.h>
#include <unistd.h>
#include <stdlib.h>

void play_sound(Uint8* sound, Uint32 len);

SDL_AudioDeviceID deviceId;

int main(void) {
    // Init SDL engine
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    
    //Load WAV
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    
    SDL_LoadWAV("../audio/select_new.wav", &wavSpec, &wavBuffer, &wavLength);
    
    // Load effects
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    char str[100];
    while(str[0] != 'q') {
        printf("ready\n");
        scanf("%s", str);
        getchar();
        play_sound(wavBuffer, wavLength);
    }

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();

}

void play_sound(Uint8* sound, Uint32 len) {

    int success = SDL_QueueAudio(deviceId, sound, len);
    SDL_PauseAudioDevice(deviceId, 0);

}