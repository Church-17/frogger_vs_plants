// Include libs
#include "struct.h"

// Define constant
#define ENTER 10
#define DIFF_CAPITAL 32

// Define variadic macros
#define wattrprintw(win, attr, args...) wattron(win, attr); wprintw(win, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, args...) wmove(win, row, col); wattrprintw(win, attr, args) // Move & print string with attribute

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
void wctrprintw(WINDOW* win, int row, str fstr);
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr);
void unwin(WINDOW* win);