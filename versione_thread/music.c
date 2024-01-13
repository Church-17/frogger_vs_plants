// Include libs
#include <unistd.h>
#include "music.h"
#include "res.h"
#include "utils.h"
#include "struct_proto.h"

// Define constant
#define MAX_VOLUME 32768

void play_sound(int sound_id) {
    // Init vars
    static const str sound[N_SOUND_EFFECTS] = {"dead_plant", "fire", "l_manche", "score", "select", "spawn_plant", "start_game", "w_manche"};
    if(VOL_EFCT_SET == 0) { // If volume == 0 don't play anything
        return;
    }
    char cmd[LIM_STR_BUFF];

    // Run mp3 player with scaled volume
    sprintf(cmd, "mpg123 -f%d ./audio/%s.mp3 2>/dev/null &", VOL_EFCT_SET*MAX_VOLUME/10, sound[sound_id]);
    system(cmd);
}

void play_music(int music_id) {
    // Init vars
    static const str music[N_MUSICS] = {"best", "easy", "hard", "l_game", "medium", "menu", "w_game"};
    static const bool loop[N_MUSICS] = {FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE};
    if(VOL_MUS_SET == 0) { // If volume == 0 don't play anything
        return;
    }
    char cmd[LIM_STR_BUFF];

    stop_music(); // Stop music
    if(loop[music_id]) { // If the music wanted need to be played in loop...
        sprintf(cmd, "mpg123 -f%d --loop -1 ./audio/%s.mp3 2>/dev/null &", VOL_MUS_SET*MAX_VOLUME/10, music[music_id]);
    } else {
        sprintf(cmd, "mpg123 -f%d ./audio/%s.mp3 2>/dev/null &", VOL_MUS_SET*MAX_VOLUME/10, music[music_id]);
    }
    
    if (system(cmd) != 0) {
        quit(ERR_PLAY_MUSIC);
    }
}

// Stop all sounds
void stop_music(void) {
    system("skill -SIGKILL -c mpg123");
}

// Pause all sounds
void pause_music(void) {
    system("skill -SIGSTOP -c mpg123");
}

// Resume all paused sounds
void resume_music(void) {
    system("skill -SIGCONT -c mpg123");
}
