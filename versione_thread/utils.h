// Include libs
#include <stdlib.h>
#include <curses.h>
#include "struct.h"

// Define constant
#define LEN_STR_INT 12
#define ENTER 10
#define CAPITAL_SHIFT 32
#define KEY_0 '0'
#define KEY_9 '9'
#define READ "r"
#define WRITE "w"

// Define error codes
#define NO_ERR 0
#define ERR_ALLOC 1
#define ERR_COLOR 2

// Define macros
#define max(n1, n2) ((n1) > (n2) ? (n1) : (n2))
#define alloc(type, var, n) var = (type*) malloc((n)*sizeof(type)); if(var == NULL) quit(ERR_ALLOC); // malloc with error-handle
#define dalloc(type, var, n) type* alloc(type, var, n)
#define wattrprintw(win, attr, args...) wattron(win, attr); wprintw(win, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, args...) wmove(win, row, col); wattrprintw(win, attr, args) // Move & print string with attribute

// Define structs
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
int mod(int n1, int n2);
int max_strlen(List_str strings, int min_len);
bool is_char_in(char ch, char first, char last);
List_str dict_to_list(str* obj, int* ind, int len);
WINDOW* newctrwin(int n_rows, int n_cols);
void wctrprintw(WINDOW* win, int row, str fstr);
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr);
void wfattrprintw(WINDOW* win, attr_t attr, str fstr);
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr);
void unwin(WINDOW* win);
void quit(int err_code);