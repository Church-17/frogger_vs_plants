// installare le librerie libvlccore-dev libvlc-dev
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>
#include <unistd.h>

void play_sound(char* path, int duration);

int main(int argc, char **argv)
{
    
    play_sound("invalid_keypress.mp3", 8);

    return 0;
}

void play_sound(char* path, int duration) {
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    // load the vlc engine
    inst = libvlc_new(0, NULL);

    // create a new item
    m = libvlc_media_new_path(inst, path);

    // create a media play playing environment
    mp = libvlc_media_player_new_from_media(m);

    // no need to keep the media now
    libvlc_media_release(m);

    // play the media_player
    libvlc_media_player_play(mp);

    sleep(duration);//play the audio 100s

    // stop playing
    // libvlc_media_player_stop(mp);

    // free the media_player
    libvlc_media_player_release(mp);

    libvlc_release(inst);
}