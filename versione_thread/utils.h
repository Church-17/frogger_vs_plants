// Include libs
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "struct.h"

// Define constant
#define READ "r"
#define WRITE "w"
#define NO_CURS 0
#define LEN_STR_INT 12
#define LIM_STR_BUFF 100 // Max len of string
#define CAPITAL_SHIFT ('a'-'A')
#define ENTER 10
#define KEY_0 '0'
#define KEY_9 '9'
#define KEY_W 'W'
#define KEY_A 'A'
#define KEY_S 'S'
#define KEY_D 'D'
#define KEY_w 'w'
#define KEY_a 'a'
#define KEY_s 's'
#define KEY_d 'd'

// Define error codes
#define NO_ERR 0
#define ERR_ALLOC 1
#define ERR_COLOR 2

// Define macros
#define alloc(type, var, n) if((var = (type*) malloc((n)*sizeof(type))) == NULL) quit(ERR_ALLOC) // malloc with error-handle
#define dalloc(type, var, n) type* var; alloc(type, var, n) // Define and alloc

// Print in center
#define wctrprintw(win, row, args...) {\
    char tmpstr[LIM_STR_BUFF];\
    sprintf(tmpstr, args);\
    mvwprintw(win, row, (win->_maxx - strlen(tmpstr))/2, "%s", tmpstr);\
}

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
struct Point {
    int x;
    int y;
};

// Function prototypes
int max(int n1, int n2);
int mod(int n1, int n2);
int rand_range(int min0, int max0);
int max_strlen(List_str strings, int min_len);
List_str dict_to_list(str* obj, int* ind, int len);
void sort_dict(Dict_str_int* dict);
void mv_win(WINDOW* win, int new_y, int new_x);
void unwin(WINDOW* win);
void quit(int err_code);