// Include libs
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "music.h"
#include "res.h"
#include "utils.h"
#include "struct_proto.h"

// Define constant
#define MAX_VOLUME 32768

void play_sound(int sound_id) {
    static const str sound[N_SOUND_EFFECTS] = {"dead_plant", "fire", "l_manche", "score", "select", "spawn_plant", "start_game", "w_manche"};
    char cmd[LIM_STR_BUFF];
    sprintf(cmd, "mpg123 -f%d ./audio/%s.mp3 2>/dev/null &", VOL_EFCT_SET*MAX_VOLUME/10, sound[sound_id]);
    system(cmd);
}

void play_music(int music_id) {
    static const str music[N_MUSICS] = {"best", "easy", "hard", "l_game", "medium", "menu", "w_game"};
    if(VOL_MUS_SET == 0) {
        return;
    }
    system("skill -SIGKILL -c mpg123");
    if(music_id == STOP_MUSIC) {
        return;
    }
    char cmd[LIM_STR_BUFF];
    sprintf(cmd, "mpg123 -f%d ./audio/%s.mp3 2>/dev/null &", VOL_MUS_SET*MAX_VOLUME/10, music[music_id]);
    system(cmd);
}