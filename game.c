// Include libs
#include "main.h"
#include "menu.h"
#include "game.h"
#include "str.h"
#include "struct.h"
#include "utils.h"
#include "manche.h"

// Function prototypes
int play(void);

// Start demo in main_scr
void menu_bg(void) {
    wctrprintw(main_scr, 1, STR_TITLE);
}

// Manage more games
bool game() {
    // Init vars
    bool do_play = TRUE, do_quit = FALSE; // Flag
    int game_ret;

    while(do_play) {

        game_ret = play();

        switch(game_ret) {
            case OVER_RETR_ID: // Play
                break;

            case OVER_BACK_ID: // Back to menu
                do_play = FALSE;
                break;

            case OVER_QUIT_ID:
                do_play = FALSE;
                do_quit = TRUE;
                break;

            default:
                break;
        }
    }
    wclear(main_scr);
    wrefresh(main_scr);
    return do_quit;
}

// Play a game handling more manche, return Game_Over_Menu_Index
int play(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    int i, score = 0, n_lifes = N_LIFES;
    int manche_remained_time[N_HOLES] = {0}; // Array with the remained time of each manche

    // Loop for play n manche saving the remained time and updating lifes
    for(i = 0; i < N_HOLES && n_lifes; i++) {
        manche_remained_time[i] = play_manche(holes_occupied, n_lifes);
        switch(manche_remained_time[i]) {
            case MANCHE_LOST:
                n_lifes--;
                if(n_lifes > 0) {
                    i--;
                }
                break;

            case MANCHE_RETR:
                return OVER_RETR_ID;
                break;

            case MANCHE_CLOSE:
                return OVER_BACK_ID;
                break;

            case MANCHE_QUIT:
                return OVER_QUIT_ID;
                break;

            default: break;
        }
    }

    for(i = 0; i < N_HOLES; i++) {
        score += manche_remained_time[i];
    }
    score *= n_lifes;
    return gameover_menu(score);
}
