// Include libs
#include <curses.h>
#include <pthread.h>
#include <locale.h>
#include "menu.h"
#include "res.h"
#include "utils.h"

// Define constant
#define COLOR_SILVER 8
#define COLOR_BRONZE 9

// Main
int main(void) {
    // Setup screen
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    
    // Setup colors
    if(has_colors() == FALSE) {	// Check color support
        endwin();
		printf("Your terminal does not support color\n");
        getchar();
		exit(1);
	}
    start_color(); // Enable colors
    init_color(COLOR_YELLOW, 1000, 1000, 0);
    init_pair(GOLD_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_color(COLOR_SILVER, 700, 700, 700);
    init_pair(SILVER_PAIR, COLOR_SILVER, COLOR_BLACK);
    init_color(COLOR_BRONZE, 820, 600, 200);
    init_pair(BRONZE_PAIR, COLOR_BRONZE, COLOR_BLACK);
    
    // Read settings from file
    rd_settings();

    // Main loop
    while(TRUE) {
        home_menu();
    }
}
