// Include libs
#include <curses.h>
#include <string.h>
#include "utils.h"

// Define constant
#define TITLE "PAC-MAN"
#define BOX_PADN 2 // North padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADS 1 // South padding of the box
#define N_OPT_HOME 5
#define OPTS_HOME {"New game", "Best scores", "Game settings", "Credits", "Quit"}
#define Y_DIM_HOME (N_OPT_HOME + BOX_PADN + BOX_PADS)
#define X_DIM_HOME 20

// Home Menu function
int home_menu(Point scr_max) {
    // Define variables
    int key, hl = 0, old_hl;
    char* options[N_OPT_HOME] = OPTS_HOME;
    WINDOW* home_menu_win = newwin(Y_DIM_HOME, X_DIM_HOME, (scr_max.y - Y_DIM_HOME)/2, (scr_max.x - X_DIM_HOME)/2);

    // Window setup
    keypad(home_menu_win, TRUE); // Enable function keys listener
    box(home_menu_win, 0, 0); // Box the window
    wctrprintw(home_menu_win, 0, TITLE);
    mvwprintw(home_menu_win, hl+BOX_PADN, BOX_PADW, " ");
    wattrprintw(home_menu_win, A_STANDOUT, "%s", options[hl]);
    for (int i = 1; i < N_OPT_HOME; i++) {
        wmvattrprintw(home_menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, "%c", options[i][0]);
        wprintw(home_menu_win, "%s", &(options[i][1]));
    }

    // Main loop
    while(TRUE) {
        old_hl = hl; // Track old highlight

        // Get the typed key and select the right option
        key = wgetch(home_menu_win);
        switch (key) {
            case KEY_UP:
            case KEY_LEFT:
            case KEY_PPAGE:
            case 'w':
            case 'a':
            case 'W':
            case 'A':
                if (--hl == -1) {
                    hl = N_OPT_HOME - 1;
                }
                break;
            
            case KEY_DOWN:
            case KEY_RIGHT:
            case KEY_NPAGE:
            case 's':
            case 'd':
            case 'S':
            case 'D':
                if (++hl == N_OPT_HOME) {
                    hl = 0;
                }
                break;

            case KEY_HOME:
            case 'n':
            case 'N':
                hl = 0;
                break;

            case 'b':
            case 'B':
                hl = 1;
                break;

            case 'g':
            case 'G':
                hl = 2;
                break;
            
            case 'c':
            case 'C':
                hl = 3;
                break;

            case KEY_END:
            case 'q':
            case 'Q':
                hl = 4;
                break;

            case ESC:
                return 4;

            case ENTER:
                return hl;

            default:
                break;
        }

        // Update highlighted & non-highlighted option
        mvwprintw(home_menu_win, hl+BOX_PADN, BOX_PADW, " ");
        wattrprintw(home_menu_win, A_STANDOUT, "%s", options[hl]);
        if(hl != old_hl) {
            wmvattrprintw(home_menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, "%c", options[old_hl][0]);
            wprintw(home_menu_win, "%s ", &(options[old_hl][1]));
        }
    }
}