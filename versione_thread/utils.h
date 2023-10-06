// Include libs
#include "struct.h"

// Define constant
#define ENTER 10

// Define macros
// - Print
#define wattrprintw(win, attr, args...) wattron(win, attr); wprintw(win, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, args...) wmove(win, row, col); wattrprintw(win, attr, args) // Move & print string with attribute
#define wctrprintw(win, row, fs) mvwprintw(win, row, (win->_maxx - strlen(fs))/2, "%s", fs) // Print string centered
#define wctrattrprintw(win, row, attr, fs) wmvattrprintw(win, row, (win->_maxx - strlen(fs))/2, attr, "%s", fs) // Prints string centered with attribute
// - Delete window
#define unwin(win) werase(win); wrefresh(win); delwin(win); clear(); refresh()

// Define structs
struct Point {
    int y;
    int x;
};
struct List_str {
    str* list;
    int len;
};

// Function prototypes
int max_strlen(List_str strings, int min_len);