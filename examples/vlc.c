#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <vlc/libvlc.h>
#include <unistd.h>

void play_sound(void);


libvlc_instance_t *vlc;
libvlc_media_t *vlc_media;
libvlc_media_player_t *vlc_mp;

int main(void) {
    vlc = libvlc_new(0, NULL);
    vlc_media = libvlc_media_new_path(vlc, "../audio/select.mp3");
    vlc_mp = libvlc_media_player_new_from_media(vlc_media);
    libvlc_media_release(vlc_media);

    
    while(getchar() != 'q') {
        play_sound();
    }

    libvlc_media_player_release(vlc_mp);
    libvlc_release(vlc);
    return 0;
}

void play_sound(void) {
    libvlc_media_player_play(vlc_mp);
}