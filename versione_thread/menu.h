// Include libs
#include <curses.h>
#include <string.h>
#include "struct.h"

// Define constant
#define N_OPT_HOME 4
#define Y_DIM_HOME (N_OPT_HOME+2)
#define X_DIM_HOME 20

// Home Menu function
int home_menu(Point scr_max) {
    // Define variables
    int i, key, highlight = 0;
    char* options[N_OPT_HOME] = {"New game", "Settings", "Cretits", "Quit"};
    WINDOW* home_win = newwin(Y_DIM_HOME, X_DIM_HOME, (scr_max.y - Y_DIM_HOME)/2, (scr_max.x - X_DIM_HOME)/2);

    // Window settings
    box(home_win, 0, 0); // Box the window
    keypad(home_win, TRUE); // Enable function keys listener

    // Main loop
    while(TRUE) {
        // Update the option view
        for (i = 0; i < N_OPT_HOME; i++) {
            if (highlight == i) {
                wattron(home_win, A_REVERSE); // Enable highlight for the selection
            }
            wattron(home_win, A_UNDERLINE); // Enable underline for first letter
            mvwprintw(home_win, i+1, 1, "%c", options[i][0]); // Print first letter
            wattroff(home_win, A_UNDERLINE); // Disable underline
            wprintw(home_win, "%s", &(options[i][1])); // Print option
            wattroff(home_win, A_REVERSE); // Disable highlight
        }

        // Get the typed key and select the right option
        key = wgetch(home_win);
        switch (key) {
            case KEY_UP:
            case KEY_LEFT:
                if (--highlight == -1)
                    highlight = N_OPT_HOME - 1;
                break;
            
            case KEY_DOWN:
            case KEY_RIGHT:
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