// Include libs
#include <locale.h>
#include "main.h"
#include "menu.h"
#include "game.h"
#include "str.h"
#include "utils.h"
#include "struct.h"

// Inter-object vars
WINDOW* main_scr = NULL;

// Function prototypes
bool check_term(WINDOW* win);

// Main
int main(void) {
    srand(time(NULL)); // Random seed

    // Setup screen
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(NO_CURS); // Remove cursor
    cbreak(); // Don't wait ENTER in getch()
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
    init_color(COLOR_BLUE, 0, 200, 500);
    init_color(COLOR_MAGENTA, 600, 0, 600);
    init_color(COLOR_CYAN, 200, 600, 900);
    init_color(COLOR_GREY, 600, 600, 600);
    init_color(COLOR_BROWN, 800, 600, 200);
    init_color(COLOR_PURPLE, 200, 0, 400);
    // Set needed color pairs (text, bg)
    init_pair(WHITE_BLACK_ID, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK_ID, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_BLACK_ID, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_BLACK_ID, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_BLACK_ID, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA_BLACK_ID, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_BLACK_ID, COLOR_CYAN, COLOR_BLACK);
    init_pair(GREY_BLACK_ID, COLOR_GREY, COLOR_BLACK);
    init_pair(BROWN_BLACK_ID, COLOR_BROWN, COLOR_BLACK);
    init_pair(GREEN_YELLOW_ID, COLOR_GREEN, COLOR_YELLOW);
    init_pair(MAGENTA_GREEN_ID, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(GREEN_PURPLE_ID, COLOR_GREEN, COLOR_PURPLE);
    init_pair(GREEN_BLUE_ID, COLOR_GREEN, COLOR_BLUE);
    init_pair(GREEN_GREY_ID, COLOR_GREEN, COLOR_GREY);
    init_pair(GREEN_RED_ID, COLOR_GREEN, COLOR_RED);
    
    // Read settings from file
    rd_settings();

    // Check terminal size
    check_term(NULL);

    // Init vars
    bool do_quit = FALSE;
    int chosen;
    main_scr = new_ctrwin(MAIN_ROWS, MAIN_COLS);

    // Start demo in bg
    print_background();
    wrefresh(main_scr);

    // Main loop
    while(!do_quit) {
        chosen = home_menu();
        print_background();
        wrefresh(main_scr);
        switch(chosen) {
            case HOME_GAME_ID: // Game
                do_quit = game();
                break;

            case HOME_BEST_ID: // Best scores
                best_scores_menu();
                break;

            case HOME_SETT_ID: // Settings
                settings_menu();
                break;

            case HOME_CRED_ID: // Credits
                credits_menu();
                break;

            case HOME_QUIT_ID:
                do_quit = TRUE;
                break;
        }
        // Redraw
        print_background();
        wrefresh(main_scr);
    }
    quit(NO_ERR);
}

// Check if term is large enough, return TRUE if it is not large enough, FALSE if it is
bool check_term(WINDOW* win) {
    if(LINES <= MAIN_ROWS || COLS <= MAIN_COLS) { // Check if terminal size is enough
        if(main_scr != NULL) { // If a window is passed, move it at top left
            mv_win(main_scr, 0, 0);
        }
        if(win != NULL) { // If a window is passed, move it at top left
            mv_win(win, 0, 0);
        }
        WINDOW* err_win = new_win(0, 0, 0, 0); // New full window
        wattron(err_win, COL1);
        while(LINES <= MAIN_ROWS || COLS <= MAIN_COLS) {
            mvwprintw(err_win, 0, 0, "%s", STR_EXTEND);
            mvwprintw(err_win, 1, 0, "%s: %d x %d    ", STR_MINIMUM, MAIN_ROWS + 1, MAIN_COLS + 1);
            mvwprintw(err_win, 2, 0, "%s: %d x %d    ", STR_ACTUAL, LINES, COLS);
            wgetch(err_win);
        }
        unwin(err_win);
        return TRUE;
    }
    return FALSE;
}

// Procedure to call after any resize, return TRUE if term is not large enough, FALSE if it is
bool resize_proc(WINDOW* win, int dim_y, int dim_x, const Game_t* gamevar) {
    bool do_prints = check_term(win); // Check terminal size

    // Lib fix: don't stick window on axis
    if(main_scr->_maxy >= MAIN_ROWS) {main_scr->_maxy = MAIN_ROWS-1;}
    if(main_scr->_maxx >= MAIN_COLS) {main_scr->_maxx = MAIN_COLS-1;}
    if(win != NULL) {
        if(win->_maxy >= dim_y) {win->_maxy = dim_y-1;}
        if(win->_maxx >= dim_x) {win->_maxx = dim_x-1;}
    }

    // Move windows
    mv_win(main_scr, LINES/2 - MAIN_ROWS/2, COLS/2 - MAIN_COLS/2);
    if(win != NULL) {
        mv_win(win, LINES/2 - dim_y/2, COLS/2 - dim_x/2);
    }

    // Redraw main_scr
    if(do_prints) {
        if(gamevar == NULL) {
            print_background(); // Redraw background
        } else {
            print_game(gamevar); // Redraw game
        }
    }
    wrefresh(main_scr);
    return do_prints;
}