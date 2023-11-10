// Include libs
#include <stdlib.h>
#include <curses.h>
#include "struct.h"

// Define constant
#define ENTER 10
#define CAPITAL_SHIFT 32
#define READ "r"
#define WRITE "w"

// Define macros
#define alloc(type, var, n) var = (type*) malloc((n)*sizeof(type)); if(var == NULL) error("Allocated NULL"); // malloc with error-handle
#define dalloc(type, var, n) type* alloc(type, var, n)
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
struct List_attr {
    attr_t* list;
    int len;
};
struct Dict_str_int {
    str* key;
    int* val;
    int len;
};

// Function prototypes
int max(int n1, int n2);
int mod(int n1, int n2);
int max_strlen(List_str strings, int min_len);
str int_to_str(int num);
bool is_char_in(char ch, char first, char last);
List_str dict_to_list(Dict_str_int dict);
WINDOW* newctrwin(int n_rows, int n_cols);
void wctrprintw(WINDOW* win, int row, str fstr);
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr);
void wfattrprintw(WINDOW* win, attr_t attr, str fstr);
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr);
void unwin(WINDOW* win);
void error(str err_str);