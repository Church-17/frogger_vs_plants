// Include libs
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "menu.h"

// Define constant
#define TITLE " PAC-MAN "

// Prototypes
void home_menu();

// Main
int main(void) {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    start_color(); // Enable colors
    
    home_menu(); // Call Home Menu

    endwin(); // End window
    return 0;
}

// Home Menu function
void home_menu() {
    int chosen;
    str list[N_OPTS_HOME] = {"New game", "Best scores", "Settings", "Credits", "Quit"};
    List_str opts;
    opts.list = list;
    opts.len = N_OPTS_HOME;
    while(TRUE) {
        chosen = menu(TITLE, opts, TRUE);
        switch(chosen) {
            case 0:
                game();
                break;
            case 1:
                best_scores();
                break;
            case 2:
                game_settings();
                break;
            case 3:
                credits_menu();
                break;
            default:
                return;
        }
    }
}