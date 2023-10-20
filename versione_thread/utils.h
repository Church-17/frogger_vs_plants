// Include libs
#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include "struct.h"

// Define constant
#define ENTER 10
#define DIFF_CAPITAL 32

// Define macros
#define dalloc(type, var, n) type* var = (type*) malloc((n)*sizeof(type))
#define alloc(type, n) (type*) malloc((n)*sizeof(type))
#define wattrprintw(win, attr, args...) wattron(win, attr); wprintw(win, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, args...) wmove(win, row, col); wattrprintw(win, attr, args) // Move & print string with attribute
#define newctrwin(n_rows, n_cols) newwin(n_rows, n_cols, (LINES - (n_rows))/2, (COLS - (n_cols))/2) // New centered window

// Define structs
struct Point {
    int y;
    int x;
};
struct List_str {
    str* list;
    int len;
};
struct UserScore {
    str user;
    int score;
};

// Function prototypes
int max_strlen(List_str strings, int min_len);
str int_to_str(int num);
void wctrprintw(WINDOW* win, int row, str fstr);
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr);
void wfattrprintw(WINDOW* win, attr_t attr, str fstr);
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr);
void unwin(WINDOW* win);