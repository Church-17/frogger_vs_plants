// Include libs
#include <curses.h>
#include <string.h>
#include "struct.h"

// Define constant
#define N_OPT_HOME 4
#define Y_DIM_HOME (N_OPT_HOME+2)
#define X_DIM_HOME 20

int home_menu(Point scr_max) {
    WINDOW* home_win = newwin(Y_DIM_HOME, X_DIM_HOME, (scr_max.y - Y_DIM_HOME)/2, (scr_max.x - X_DIM_HOME)/2);
    box(home_win, 0, 0);
    wrefresh(home_win);
    keypad(home_win, TRUE);

    int i, key, highlight = 0;
    char* options[N_OPT_HOME] = {"New game", "Settings", "Cretits", "Quit"};

    while(TRUE) {
        for (i = 0; i < N_OPT_HOME; i++) {    // dispone le opzioni evidenziandone una a seconda di highlight
            if (highlight == i) {
                wattron(home_win, A_REVERSE);
            }
            wattron(home_win, A_UNDERLINE);
            mvwprintw(home_win, i+1, 1, "%c", options[i][0]);
            wattroff(home_win, A_UNDERLINE);
            wprintw(home_win, "%s", &(options[i][1]));
            wattroff(home_win, A_REVERSE);
        }

        key = wgetch(home_win);
        switch (key) {
            case KEY_UP:
                if (--highlight == -1)
                    highlight = N_OPT_HOME - 1;
                break;
            
            case KEY_DOWN:
                if (++highlight == N_OPT_HOME)
                    highlight = 0;
                break;

            case 'n':
                highlight = 0;
                break;

            case 's':
                highlight = 1;
                break;
            
            case 'c':
                highlight = 2;
                break;

            case 'q':
                highlight = 3;
                break;

            case ENTER:
                return highlight;

            default:
                break;
        }
    }
}