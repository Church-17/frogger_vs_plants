// Include libs
#include <curses.h>
#include <string.h>
#include "utils.h"

// Define constant
#define N_OPT_HOME 4
#define OPTS_HOME {"New game", "Game settings", "Credits", "Quit"}
#define Y_DIM_HOME (N_OPT_HOME+2)
#define X_DIM_HOME 20

// Home Menu function
int home_menu(Point scr_max) {
    // Define variables
    int i, key, highlight = 0;
    char* options[N_OPT_HOME] = OPTS_HOME;
    WINDOW* home_win = newwin(Y_DIM_HOME, X_DIM_HOME, (scr_max.y - Y_DIM_HOME)/2, (scr_max.x - X_DIM_HOME)/2);

    // Window settings
    keypad(home_win, TRUE); // Enable function keys listener

    // Main loop
    while(TRUE) {
        // Update the option view
        wclear(home_win);
        box(home_win, 0, 0); // Box the window
        for (i = 0; i < N_OPT_HOME; i++) {
            if (highlight == i) {
                wattron(home_win, A_STANDOUT); // Enable highlight for the selection
                wmove(home_win, i+1, 2);
            } else {
                wmove(home_win, i+1, 1);
            }
            wattron(home_win, A_UNDERLINE); // Enable underline for first letter
            wprintw(home_win, "%c", options[i][0]); // Print first letter
            wattroff(home_win, A_UNDERLINE); // Disable underline
            wprintw(home_win, "%s", &(options[i][1])); // Print option
            wattroff(home_win, A_STANDOUT); // Disable highlight
        }

        // Get the typed key and select the right option
        key = wgetch(home_win);
        switch (key) {
            case KEY_UP:
            case KEY_LEFT:
            case 'w':
            case 'a':
            case 'W':
            case 'A':
                if (--highlight == -1)
                    highlight = N_OPT_HOME - 1;
                break;
            
            case KEY_DOWN:
            case KEY_RIGHT:
            case 's':
            case 'd':
            case 'S':
            case 'D':
                if (++highlight == N_OPT_HOME)
                    highlight = 0;
                break;

            case 'n':
            case 'N':
                highlight = 0;
                break;

            case 'g':
            case 'G':
                highlight = 1;
                break;
            
            case 'c':
            case 'C':
                highlight = 2;
                break;

            case 'q':
            case 'Q':
                highlight = 3;
                break;

            case ENTER:
                return highlight;

            default:
                break;
        }
    }
}