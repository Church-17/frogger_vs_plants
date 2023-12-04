// Include libs
#include "main.h"
#include "menu.h"
#include "game.h"
#include "str.h"
#include "struct.h"
#include "utils.h"
#include "manche.h"
#include "sprites.h"

// Function prototypes
int play(void);

// Start demo in main_scr
void print_bg_frog(void) {
    int i;
    for(i = LINE_BANK_1; i < LINE_RIVER; i++) {
        mvwaprintw(main_scr, i, 0, GREEN_PURPLE, "%*c", MAIN_COLS, ' ');
    }
    for(i = LINE_RIVER; i < LINE_BANK_2; i++) {
        mvwaprintw(main_scr, i, 0, GREEN_BLUE, "%*c", MAIN_COLS, ' ');
    }
    for(i = LINE_BANK_2; i < MAIN_ROWS; i++) {
        mvwaprintw(main_scr, i, 0, GREEN_PURPLE, "%*c", MAIN_COLS, ' ');
    }
    int cols_bg[FROG_Y_DIM] = {COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE};
    print_frog(main_scr, LINE_BANK_2, MAIN_COLS/2-FROG_X_DIM/2, cols_bg);
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
