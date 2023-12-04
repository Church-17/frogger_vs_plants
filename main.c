// Include libs
#include <locale.h>
#include <time.h>
#include "main.h"
#include "menu.h"
#include "game.h"
#include "str.h"
#include "utils.h"

// Inter-object vars
WINDOW* main_scr = NULL;

// Global vars
bool in_game_status = FALSE;

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
    init_pair(GREEN_BLACK_ID, COLOR_GREEN, COLOR_BLACK);
    init_pair(GREEN_YELLOW_ID, COLOR_GREEN, COLOR_YELLOW);
    init_pair(MAGENTA_GREEN_ID, COLOR_MAGENTA, COLOR_GREEN);
    
    // Read settings from file
    rd_settings();

    // Check terminal size
    check_term(NULL);

    // Init vars
    bool do_quit = FALSE;
    int chosen;
    main_scr = new_ctrwin(MAIN_ROWS, MAIN_COLS);

    // Start demo in bg
    menu_bg();
    wrefresh(main_scr);

    // Main loop
    while(!do_quit) {
        chosen = home_menu();
        redrawwin(main_scr);
        wrefresh(main_scr);
        switch(chosen) {
            case HOME_GAME_ID: // Game
                in_game_status = TRUE;
                do_quit = game();
                in_game_status = FALSE;
                menu_bg();
                break;

            case HOME_BEST_ID: // Best scores
                best_scores_menu();
                redrawwin(main_scr);
                break;

            case HOME_SETT_ID: // Settings
                settings_menu();
                redrawwin(main_scr);
                break;

            case HOME_CRED_ID: // Credits
                credits_menu();
                redrawwin(main_scr);
                break;

            case HOME_QUIT_ID:
                do_quit = TRUE;
                break;

            default:
                break;
        }
        // Redraw 
        wrefresh(main_scr);
    }

    quit(NO_ERR);
}

// Check if term is large enough, return TRUE if it is not large enough, FALSE if it is
bool check_term(WINDOW* win) {
    if(LINES < MAIN_ROWS || COLS < MAIN_COLS) { // Check if terminal size is enough
        if(main_scr != NULL) { // If a window is passed, move it at top left
            mv_win(main_scr, 0, 0);
        }
        if(win != NULL) { // If a window is passed, move it at top left
            mv_win(win, 0, 0);
        }
        WINDOW* err_win = new_win(0, 0, 0, 0); // New full window
        wattron(err_win, COL1);
        mvwprintw(err_win, 0, 0, "%s", STR_EXTEND);
        mvwprintw(err_win, 1, 0, "%s: %d x %d    ", STR_MINIMUM, MAIN_ROWS, MAIN_COLS);
        while(LINES < MAIN_ROWS || COLS < MAIN_COLS) {
            mvwprintw(err_win, 2, 0, "%s: %d x %d    ", STR_ACTUAL, LINES, COLS);
            wgetch(err_win);
        }
        unwin(err_win);
        return TRUE;
    }
    return FALSE;
}

// Procedure to call after any resize, return TRUE if term is not large enough, FALSE if it is
bool resize_proc(WINDOW* win, int dim_y, int dim_x) {
    bool do_prints = check_term(win); // Check terminal size

    // Lib fix: don't stick window on axis
    if(main_scr->_maxy >= MAIN_ROWS) main_scr->_maxy = MAIN_ROWS-1;
    if(main_scr->_maxx >= MAIN_COLS) main_scr->_maxx = MAIN_COLS-1;
    if(win->_maxy >= dim_y) win->_maxy = dim_y-1;
    if(win->_maxx >= dim_x) win->_maxx = dim_x-1;

    // Move windows
    mv_win(main_scr, LINES/2 - MAIN_ROWS/2, COLS/2 - MAIN_COLS/2);
    mv_win(win, LINES/2 - dim_y/2, COLS/2 - dim_x/2);

    // Redraw main_scr
    if(do_prints) {
        if(in_game_status) {
            ; // Redraw game
        } else {
            menu_bg(); // Redraw demo
        }
    }
    wrefresh(main_scr);
    return do_prints;
}