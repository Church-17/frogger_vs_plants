// Include libs
#include <curses.h>
#include <pthread.h>
#include <locale.h>
#include "menu.h"
#include "res.h"
#include "utils.h"

// Define constant
#define COLOR_GREY 8
#define COLOR_BROWN 9

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
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_WHITE, 800, 800, 800);
    init_color(COLOR_YELLOW, 900, 900, 0);
    init_color(COLOR_GREY, 600, 600, 600);
    init_color(COLOR_BROWN, 800, 600, 200);
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREY, COLOR_BLACK);
    init_pair(3, COLOR_BROWN, COLOR_BLACK);
    
    // Read settings from file
    rd_settings();

    // Main loop
    while(TRUE) {
        home_menu();
    }
}
