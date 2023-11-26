// Include libs
#include "main.h"
#include "menu.h"
#include "game.h"
#include "res.h"
#include "utils.h"

// Define constant
#define TOP_BAR_ROWS 3
#define USR '#'
#define POINT '$'
#define N_POINT 5
#define CLOSE_GAME 'q'
#define PAUSE_GAME 'p'

// Function prototypes
int play(void);

// Start demo in main_scr
void meun_bg(void) {
    wctrprintw(main_scr, 1, TITLE);
    box(main_scr, 0, 0);
}

// Manage more games
bool game(void) {
    // Init vars
    bool do_play = TRUE, do_quit = FALSE; // Flag
    int game_ret;

    while(do_play) {
        in_game_status = TRUE; // Update game status
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
    in_game_status = FALSE; // Update game status
    return do_quit;
}

int play(void) {
    WINDOW* top_bar = newwin(TOP_BAR_ROWS, COLS, 0, 0);
    WINDOW* game_scr = newwin(LINES-TOP_BAR_ROWS, COLS, TOP_BAR_ROWS, 0);
    keypad(game_scr, TRUE);
    box(game_scr, ACS_VLINE, ACS_HLINE);
    int i, key, ret, x = game_scr->_maxx/2, y = game_scr->_maxy/2, point_x[N_POINT], point_y[N_POINT], score = 0;  // Inizio al centro
    mvwaddch(game_scr, y, x, USR);
    wctrprintw(top_bar, TOP_BAR_ROWS/2, "%s: %d", SCORE, score);
    for(i = 0; i < N_POINT; i++) {
        point_x[i] = rand_range(1, game_scr->_maxx);
        point_y[i] = rand_range(1, game_scr->_maxy);
        mvwaddch(game_scr, point_y[i], point_x[i], POINT);
    }
    wrefresh(top_bar);
    wrefresh(game_scr);
    while(TRUE) {
        key = wgetch(game_scr);
        mvwaddch(game_scr, y, x, ' ');
        switch(key) {
            case KEY_UP:
                if(y > 1) {
                    y -= 1;
                }
                break;

            case KEY_DOWN:
                if(y < game_scr->_maxy - 1) {
                    y += 1;
                }
                break;

            case KEY_LEFT:
                if(x > 1) {
                    x -= 1;
                }
                break;

            case KEY_RIGHT:
                if(x < game_scr->_maxx - 1) {
                    x += 1;
                }
                break;

            case PAUSE_GAME:
                ret = pause_menu();
                switch (ret) {
                    case PAUSE_RES_ID:
                        break;
                    
                    case PAUSE_RETR_ID:
                        return OVER_RETR_ID;

                    case PAUSE_BACK_ID:
                        unwin(top_bar);
                        unwin(game_scr); 
                        return OVER_BACK_ID;

                    case PAUSE_QUIT_ID:                    
                        return OVER_QUIT_ID;
                }
                redrawwin(game_scr); // Restore element under menu
                break;

            case CLOSE_GAME:
                ret = gameover_menu(score);
                unwin(top_bar);
                unwin(game_scr);
                return ret;
        }
        for(i = 0; i < N_POINT; i++) {
            if(x == point_x[i] && y == point_y[i]) {
                score++;
                point_x[i] = rand_range(1, game_scr->_maxx);
                point_y[i] = rand_range(1, game_scr->_maxy);
                mvwaddch(game_scr, point_y[i], point_x[i], POINT);
                wctrprintw(top_bar, TOP_BAR_ROWS/2, "%s: %d", SCORE, score);
                wrefresh(top_bar);
            }
        }
        mvwaddch(game_scr, y, x, USR);
        wrefresh(game_scr);
    }
}