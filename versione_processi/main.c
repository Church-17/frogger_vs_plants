// Include libs
#include <locale.h>
#include <unistd.h>
#include <signal.h>
#include "main.h"
#include "menu.h"
#include "game.h"
#include "music.h"
#include "str.h"
#include "utils.h"
#include "struct.h"

// Define constants
#define NEW_GAME_SLEEP 1000

// Inter-object vars
WINDOW* main_scr = NULL;

// Function prototypes
bool check_term(WINDOW* win);
void terminate(int sig);

// Main
int main(void) {
    srand(timestamp()); // Random seed
    signal(SIGINT, &terminate);

    // Setup screen
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(NO_CURSOR); // Remove cursor
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
    init_color(COLOR_BLUE, 0, 300, 800);
    init_color(COLOR_MAGENTA, 700, 0, 700);
    init_color(COLOR_CYAN, 200, 600, 900);
    init_color(COLOR_GREY, 600, 600, 600);
    init_color(COLOR_ORANGE, 900, 600, 100);
    init_color(COLOR_PURPLE, 300, 0, 600);
    init_color(COLOR_BORDEAUX, 400, 100, 100);
    init_color(COLOR_DARKGREEN, 0, 400, 0);
    init_color(COLOR_DARKBLUE, 100, 0, 400);
    init_color(COLOR_LIGHTGREEN, 600, 900, 600);
    init_color(COLOR_PINK, 800, 500, 600);
    // Set needed color pairs (text, bg)
    init_pair(WHITE_BLACK_ID, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK_ID, COLOR_RED, COLOR_BLACK);
    init_pair(ORANGE_BLACK_ID, COLOR_ORANGE, COLOR_BLACK);
    init_pair(YELLOW_BLACK_ID, COLOR_YELLOW, COLOR_BLACK);
    init_pair(GREEN_BLACK_ID, COLOR_GREEN, COLOR_BLACK);
    init_pair(CYAN_BLACK_ID, COLOR_CYAN, COLOR_BLACK);
    init_pair(BLUE_BLACK_ID, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA_BLACK_ID, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(GREY_BLACK_ID, COLOR_GREY, COLOR_BLACK);
    init_pair(GREEN_YELLOW_ID, COLOR_GREEN, COLOR_YELLOW);
    init_pair(MAGENTA_GREEN_ID, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(GREEN_PURPLE_ID, COLOR_GREEN, COLOR_PURPLE);
    init_pair(GREEN_DARKBLUE_ID, COLOR_GREEN, COLOR_DARKBLUE);
    init_pair(GREEN_BORDEAUX_ID, COLOR_GREEN, COLOR_BORDEAUX);
    init_pair(GREEN_DARKGREEN_ID, COLOR_GREEN, COLOR_DARKGREEN);
    init_pair(BORDEAUX_DARKBLUE_ID, COLOR_BORDEAUX, COLOR_DARKBLUE);
    init_pair(DARKGREEN_DARKBLUE_ID, COLOR_DARKGREEN, COLOR_DARKBLUE);
    init_pair(LIGHTGREEN_DARKGREEN_ID, COLOR_LIGHTGREEN, COLOR_DARKGREEN);
    init_pair(WHITE_DARKGREEN_ID, COLOR_WHITE, COLOR_DARKGREEN);
    init_pair(CYAN_DARKBLUE_ID, COLOR_CYAN, COLOR_DARKBLUE);
    init_pair(WHITE_BORDEAUX_ID, COLOR_WHITE, COLOR_BORDEAUX);
    init_pair(PINK_BORDEAUX_ID, COLOR_PINK, COLOR_BORDEAUX);
    init_pair(RED_PURPLE_ID, COLOR_RED, COLOR_PURPLE);
    init_pair(RED_DARKBLUE_ID, COLOR_RED, COLOR_DARKBLUE);
    init_pair(RED_DARKGREEN_ID, COLOR_RED, COLOR_DARKGREEN);
    init_pair(RED_BORDEAUX_ID, COLOR_RED, COLOR_BORDEAUX);
    init_pair(RED_ORANGE_ID, COLOR_RED, COLOR_ORANGE);
    init_pair(CYAN_RED_ID, COLOR_CYAN, COLOR_RED);
    init_pair(DARKGREEN_PURPLE_ID, COLOR_DARKGREEN, COLOR_PURPLE);
    init_pair(WHITE_RED_ID, COLOR_WHITE, COLOR_RED);
    init_pair(WHITE_PURPLE_ID, COLOR_WHITE, COLOR_PURPLE);
    init_pair(GREEN_GREY_ID, COLOR_GREEN, COLOR_GREY);
    init_pair(RED_GREY_ID, COLOR_RED, COLOR_GREY);
    init_pair(GREEN_LIGHTGREEN_ID, COLOR_GREEN, COLOR_LIGHTGREEN);
    init_pair(RED_LIGHTGREEN_ID, COLOR_RED, COLOR_LIGHTGREEN);
    init_pair(GREEN_PINK_ID, COLOR_GREEN, COLOR_PINK);
    init_pair(RED_PINK_ID, COLOR_RED, COLOR_PINK);
    init_pair(WHITE_GREEN_ID, COLOR_WHITE, COLOR_GREEN);
    init_pair(GREEN_GREEN_ID, COLOR_GREEN, COLOR_GREEN);
    init_pair(GREEN_RED_ID, COLOR_GREEN, COLOR_RED);
    init_pair(RED_RED_ID, COLOR_RED, COLOR_RED);
    
    // Initial procedure
    rd_settings(); // Read settings from file
    check_term(NULL); // Check terminal size
    play_music(MUSIC_MENU);

    // Init vars
    bool do_quit = FALSE;
    int chosen;
    main_scr = new_ctrwin(MAIN_ROWS, MAIN_COLS);

    // Main loop
    while(!do_quit) {
        // Start demo in bg
        demo();
        wrefresh(main_scr);

        chosen = home_menu();
        switch(chosen) {
            case HOME_GAME_ID: // Game
                stop_music();
                play_sound(SOUND_START_GAME);
                demo();
                wrefresh(main_scr);
                usleep(MSEC_IN_SEC * NEW_GAME_SLEEP);
                do_quit = game();
                if(!do_quit) {
                    play_music(MUSIC_MENU);
                }
                break;

            case HOME_BEST_ID: // Best scores
                play_sound(SOUND_MENU_SELECTION);
                demo();
                wrefresh(main_scr);
                best_scores_menu();
                play_sound(SOUND_MENU_SELECTION);
                break;

            case HOME_SETT_ID: // Settings
                play_sound(SOUND_MENU_SELECTION);
                demo();
                wrefresh(main_scr);
                settings_menu();
                play_sound(SOUND_MENU_SELECTION);
                break;

            case HOME_CRED_ID: // Credits
                play_sound(SOUND_MENU_SELECTION);
                demo();
                wrefresh(main_scr);
                credits_menu();
                play_sound(SOUND_MENU_SELECTION);
                break;

            case HOME_QUIT_ID:
                do_quit = TRUE;
                break;
        }
    }
    quit(NO_ERR);
}

// Check if term is large enough, return TRUE if it is not large enough, FALSE if it is
bool check_term(WINDOW* win) {
    if(LINES < MAIN_ROWS || COLS < MAIN_COLS) { // Check if terminal size is enough
        if(main_scr != NULL) { // If main_scr exist, move it at top left
            mvwin(main_scr, 0, 0);
        }
        if(win != NULL) { // If a window is passed, move it at top left
            mvwin(win, 0, 0);
        }
        WINDOW* err_win = new_win(0, 0, 0, 0); // New full window
        wattron(err_win, COL1);
        while(LINES < MAIN_ROWS || COLS < MAIN_COLS) {
            mvwprintw(err_win, 0, 0, "%s", STR_EXTEND);
            mvwprintw(err_win, 1, 0, "%s: %d x %d    ", STR_MINIMUM, MAIN_ROWS, MAIN_COLS);
            mvwprintw(err_win, 2, 0, "%s: %d x %d    ", STR_ACTUAL, LINES, COLS);
            wgetch(err_win);
        }
        un_win(err_win);
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
    mvwin(main_scr, LINES/2 - MAIN_ROWS/2, COLS/2 - MAIN_COLS/2);
    if(win != NULL) {
        mvwin(win, LINES/2 - dim_y/2, COLS/2 - dim_x/2);
    }
    clear();
    refresh();

    // Redraw main_scr
    if(do_prints) {
        if(gamevar == NULL) {
            demo(); // Redraw background
        } else {
            print_game(gamevar); // Redraw game
        }
    }
    wrefresh(main_scr);
    return do_prints;
}

void terminate(int sig) {
    quit(NO_ERR);
}
