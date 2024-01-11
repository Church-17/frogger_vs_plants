// Include libs
#include <unistd.h>
#include "music.h"
#include "res.h"
#include "utils.h"
#include "struct_proto.h"

// Define constant
#define MAX_VOLUME 32768

void play_sound(int sound_id) {
    static const str sound[N_SOUND_EFFECTS] = {"select"};
    char cmd[LIM_STR_BUFF];
    sprintf(cmd, "mpg123 -f %d ./audio/%s.mp3 >/dev/null 2>&1 &", VOL_EFCT_SET*MAX_VOLUME/10, sound[sound_id]);
    system(cmd);
}