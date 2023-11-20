// Include libs
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "struct.h"

// Define constant
#define LEN_STR_INT 12
#define LIM_STR_BUFF 100 // Max len of string
#define ENTER 10
#define CAPITAL_SHIFT (int)('a'-'A')
#define KEY_0 (int)'0'
#define KEY_9 (int)'9'
#define KEY_w (int)'w'
#define KEY_a (int)'a'
#define KEY_s (int)'s'
#define KEY_d (int)'d'
#define KEY_W (int)'W'
#define KEY_A (int)'A'
#define KEY_S (int)'S'
#define KEY_D (int)'D'
#define READ "r"
#define WRITE "w"
#define NO_CURS 0

// Define error codes
#define NO_ERR 0
#define ERR_ALLOC 1
#define ERR_COLOR 2

// Define macros
#define max(n1, n2) ((n1) > (n2) ? (n1) : (n2))
#define mod(n1, n2) ((n1) >= 0 ? (n1) % (n2) : (n1) % (n2) + (n2))
#define rand_int(min, max) (rand() % (max-min) + min)
#define alloc(type, var, n) var = (type*) malloc((n)*sizeof(type));\
                            if(var == NULL) quit(ERR_ALLOC) // malloc with error-handle
#define dalloc(type, var, n) type* alloc(type, var, n) // Define and alloc
#define wattrprintw(win, attr, args...) wattron(win, attr);\
                                        wprintw(win, args);\
                                        wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, args...) wmove(win, row, col); wattrprintw(win, attr, args) // Move & print string with attribute
#define wctrprintw(win, row, args...) char tmpstr[LIM_STR_BUFF];\
                                      sprintf(tmpstr, args);\
                                      mvwprintw(win, row, (win->_maxx - strlen(tmpstr))/2, "%s", tmpstr)
#define wctrattrprintw(win, row, attr, args...) wattron(win, attr);\
                                                wctrprintw(win, row, args);\
                                                wattroff(win, attr)

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
int max_strlen(List_str strings, int min_len);
List_str dict_to_list(str* obj, int* ind, int len);
WINDOW* newctrwin(int n_rows, int n_cols);
void unwin(WINDOW* win);
void quit(int err_code);