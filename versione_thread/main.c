// Include libs
#include <curses.h>
#include <pthread.h>
#include <locale.h>
#include "menu.h"
#include "res.h"
#include "utils.h"

// Main
int main(void) {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    enable_colors(); // Initialize colors
    
    rd_settings();
    while(TRUE) {
        home_menu(); // Call Home Menu
    }

    endwin(); // End window
    return 0;
}
