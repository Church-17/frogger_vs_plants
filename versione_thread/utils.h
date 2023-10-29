// Include libs
#include <stdlib.h>
#include <curses.h>
#include "struct.h"

// Define constant
#define ENTER 10
#define DIFF_CAPITAL 32
// Colors
#define INDEX_WHITE_BLACK 0
#define WHITE_BLACK COLOR_PAIR(INDEX_WHITE_BLACK)
#define INDEX_YELLOW_BLACK 1
#define YELLOW_BLACK COLOR_PAIR(INDEX_YELLOW_BLACK)
#define INDEX_GREY_BLACK 2
#define GREY_BLACK COLOR_PAIR(INDEX_GREY_BLACK)
#define INDEX_BROWN_BLACK 3
#define BROWN_BLACK COLOR_PAIR(INDEX_BROWN_BLACK)

// Define macros
#define alloc(type, var, n) var = (type*) malloc((n)*sizeof(type))
#define dalloc(type, var, n) type* alloc(type, var, n)
#define wattrprintw(win, attr, args...) wattron(win, attr); wprintw(win, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, args...) wmove(win, row, col); wattrprintw(win, attr, args) // Move & print string with attribute

// Define structs
struct Point {
    int y;
    int x;
};
struct UserScore {
    str user;
    int score;
};
struct List_str {
    str* list;
    int len;
};
struct List_attr {
    attr_t* list;
    int len;
};
struct List_UserScore {
    UserScore* list;
    int len;
};

// Function prototypes
int max(int n1, int n2);
int mod(int n1, int n2);
int max_strlen(List_str strings, int min_len);
str dtos(int num);
WINDOW* newctrwin(int n_rows, int n_cols);
void wctrprintw(WINDOW* win, int row, str fstr);
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr);
void wfattrprintw(WINDOW* win, attr_t attr, str fstr);
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr);
void unwin(WINDOW* win);