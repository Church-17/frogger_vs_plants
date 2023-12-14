// Include libs
#include "manche.h"
#include "menu.h"
#include "res.h"
#include "utils.h"
#include "struct.h"

// Function prototypes
int play_game(void);

// Start demo in main_scr
void demo(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    print_background(holes_occupied);
}

// Manage more games
bool game() {
    // Init vars
    bool do_play = TRUE, do_quit = FALSE; // Flag
    int game_ret;

    // Loop for retrying play
    while(do_play) {
        game_ret = play_game();
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
    return do_quit;
}

// Play a game handling more manche, return gameover_menu index
int play_game(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    int i, score = 0, n_lifes = N_LIFES;
    int manche_remained_time[N_HOLES] = {0}; // Array with the remained time of each manche
    Game_t gamevar;

    // Loop for play n manche saving the remained time and updating lifes
    for(i = 0; i < N_HOLES && n_lifes; i++) {
        gamevar = play_manche(holes_occupied, n_lifes);
        switch(gamevar.timer) {
            case MANCHE_LOST:
                gamevar.lifes--;
                if(gamevar.lifes > 0) {
                    i--;
                } else {
                    print_lifes(gamevar.lifes);
                    wrefresh(main_scr);
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

            default:
                manche_remained_time[i] = gamevar.timer;
                break;
        }
        n_lifes = gamevar.lifes;
    }

    for(i = 0; i < N_HOLES; i++) {
        score += manche_remained_time[i];
    }
    score *= n_lifes;
    return gameover_menu(score, &gamevar);
}

void print_game(const Game_t* gamevar) {
    int i, j;

    // Print background
    print_background(gamevar->holes_occupied);

    // Print time
    print_time(gamevar->timer);

    // Print lifes
    print_lifes(gamevar->lifes);

    // Print croccodiles
    for(i = 0; i < N_WATER_STREAM; i++) {
        for(j = 0; j < MAX_CROCCODILE_PER_STREAM; j++) {
            if(gamevar->croccodiles[i][j].y >= 0) {
                print_croccodile(gamevar->croccodiles[i][j], gamevar->stream_speed > 0);
            }
        }
    }
    
    // Print frog
    print_frog(gamevar);
}
