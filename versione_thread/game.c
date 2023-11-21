// Include libs
#include "game.h"
#include "menu.h"
#include "res.h"
#include "utils.h"

// Define constant
#define TOP_BAR_ROWS 3
#define USR '#'
#define POINT '$'
#define N_POINT 5
#define CLOSE_GAME 'q'

// Function prototypes
int play(void);

void game(void) {
    int chosen, lets_play = 1;
    while(lets_play) {
        chosen = play();
        switch(chosen) {
            case OVER_RETR_ID: // Play
                break;

            case OVER_BACK_ID: // Back to menu
                lets_play = 0;
                break;

            case OVER_QUIT_ID:
                quit(NO_ERR);

            default:
                break;
        }
    }
    return;
}

int play(void) {
    WINDOW* top_bar = newwin(TOP_BAR_ROWS, COLS, 0, 0);
    WINDOW* game_scr = newwin(LINES-TOP_BAR_ROWS, COLS, TOP_BAR_ROWS, 0);
    keypad(game_scr, TRUE);
    box(game_scr, ACS_VLINE, ACS_HLINE);
    int i, key, chosen, x = game_scr->_maxx/2, y = game_scr->_maxy/2, point_x[N_POINT], point_y[N_POINT], score = 0;  // Inizio al centro
    mvwaddch(game_scr, y, x, USR);
    wctrprintw(top_bar, TOP_BAR_ROWS/2, "%s: %d", SCORE, score);
    for(i = 0; i < N_POINT; i++) {
        point_x[i] = rand_int(1, game_scr->_maxx);
        point_y[i] = rand_int(1, game_scr->_maxy);
        mvwaddch(game_scr, point_y[i], point_x[i], POINT);
    }
    wrefresh(top_bar);
    wrefresh(game_scr);
    while(1) {
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

            case CLOSE_GAME:
                chosen = gameover_menu(score);
                unwin(top_bar);
                unwin(game_scr);
                return chosen;
        }
        for(i = 0; i < N_POINT; i++) {
            if(x == point_x[i] && y == point_y[i]) {
                score++;
                point_x[i] = rand_int(1, game_scr->_maxx);
                point_y[i] = rand_int(1, game_scr->_maxy);
                mvwaddch(game_scr, point_y[i], point_x[i], POINT);
                wctrprintw(top_bar, TOP_BAR_ROWS/2, "%s: %d", SCORE, score);
                wrefresh(top_bar);
            }
        }
        mvwaddch(game_scr, y, x, USR);
        wrefresh(game_scr);
    }
}