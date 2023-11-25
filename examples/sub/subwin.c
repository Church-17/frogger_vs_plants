#include <ncurses.h>
#include "menu.h"

WINDOW* main_scr;

int main(void) {
    initscr();
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    cbreak(); // Don't wait ENTER in getch()
    start_color(); // Enable colors
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    main_scr = newwin(15, 50, (LINES-15)/2, (COLS-50)/2);
    keypad(main_scr, TRUE);
    wprintw(main_scr, "hey");
    wrefresh(main_scr);
    home_menu();

    endwin();
}