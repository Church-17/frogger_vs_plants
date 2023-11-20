// Include libs
#include <locale.h>
#include "menu.h"
#include "res.h"
#include "utils.h"

// Main
int main(void) {
    srand(time(NULL)); // Random seed

    // Setup screen
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(NO_CURS); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    
    // Setup colors
    if(has_colors() == FALSE) {	// Check color support
		quit(ERR_COLOR);
	}
    start_color(); // Enable colors
    // Set each color with RGB values (scaled on thousandth)
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_WHITE, 800, 800, 800);
    init_color(COLOR_RED, 800, 0, 0);
    init_color(COLOR_GREEN, 0, 700, 200);
    init_color(COLOR_YELLOW, 800, 800, 0);
    init_color(COLOR_BLUE, 0, 300, 900);
    init_color(COLOR_MAGENTA, 600, 0, 600);
    init_color(COLOR_CYAN, 200, 600, 900);
    init_color(COLOR_GREY, 600, 600, 600);
    init_color(COLOR_BROWN, 800, 600, 200);
    // Set needed color pairs (text, bg)
    init_pair(COL_0_ID, COLOR_WHITE, COLOR_BLACK);
    init_pair(COL_1_ID, COLOR_RED, COLOR_BLACK);
    init_pair(COL_2_ID, COLOR_GREEN, COLOR_BLACK);
    init_pair(COL_3_ID, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COL_4_ID, COLOR_BLUE, COLOR_BLACK);
    init_pair(COL_5_ID, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COL_6_ID, COLOR_CYAN, COLOR_BLACK);
    init_pair(GREY_BLACK_ID, COLOR_GREY, COLOR_BLACK);
    init_pair(BROWN_BLACK_ID, COLOR_BROWN, COLOR_BLACK);
    
    // Read settings from file
    rd_settings();

    // Main loop
    while(TRUE) {
        home_menu();
    }
}
