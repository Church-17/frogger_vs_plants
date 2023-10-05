// Include libs
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "menu.h"

// Define constant

// Main
int main(void) {
    int chosen;

    setlocale(LC_ALL, "");
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    start_color(); // Enable colors
    
    while(TRUE) {
        chosen = home_menu(); // Call Home Menu
        switch(chosen) {
            case 0:
                getch();
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
            case 4:
                endwin();
                return 0;
        }
    }

    endwin(); // End window
    return 0;
}