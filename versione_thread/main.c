// Include libs
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "menu.h"

// Define constant

// Main
int main(void) {
    int chosen;
    Point scr_dim;

    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    getmaxyx(stdscr, scr_dim.y, scr_dim.x); // Get screen dimension

    while(TRUE) {
        chosen = home_menu(scr_dim); // Call Home Menu
        switch(chosen) {
            case 0:
                getch();
                break;
            case 1:
                best_scores(scr_dim);
                break;
            case 2:
                game_settings(scr_dim);
                break;
            case 3:
                credits_menu(scr_dim);
                break;
            case 4:
                endwin();
                return 0;
        }
    }

    endwin(); // End window
    return 0;
}