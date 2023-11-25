#include <ncurses.h>

int main(void) {
    initscr();
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    keypad(stdscr, TRUE); // Enable function keys listener
    cbreak(); // Don't wait ENTER in getch()
    start_color(); // Enable colors
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    WINDOW* main_scr = newwin(10, 20, (LINES-10)/2, (COLS-20)/2);
    keypad(main_scr, TRUE);
    wprintw(main_scr, "Hey");

    WINDOW* sub = derwin(main_scr, 5, 10, (10-5)/2, (20-10)/2);
    keypad(sub, TRUE);
    wprintw(sub, "Ciao");
    redrawwin(sub);

    while(wgetch(main_scr) == KEY_RESIZE) {
        mvwin(main_scr, (LINES-10)/2, (COLS-20)/2);
        clear();
        refresh();
    }

    endwin();
}