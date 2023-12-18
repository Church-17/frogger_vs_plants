// Include libs
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct_proto.h"

// Define constant
#define READ "r"
#define WRITE "w"
#define MSEC_IN_SEC 1000
#define NO_CURS 0
#define LEN_STR_INT 12
#define LIM_STR_BUFF 100 // Max len of string
#define CAPITAL_SHIFT ('a'-'A')
#define ENTER 10

// Define error codes
#define NO_ERR 0
#define ERR_ALLOC 1
#define ERR_COLOR 2
#define ERR_WIN 3
#define ERR_FORK 4
#define ERR_PIPE 5
#define ERR_READ 6
#define ERR_WRITE 7

// Define macros
#define alloc(type, var, n) if((var = (type*) malloc((n)*sizeof(type))) == NULL) quit(ERR_ALLOC) // malloc with error-handle
#define dalloc(type, var, n) \
    type* var;\
    alloc(type, var, n) // Define and alloc

// Print in window with attribute
#define mvwaprintw(win, y, x, attr, fstr...)\
    wattron(win, attr);\
    mvwprintw(win, y, x, fstr);\
    wattroff(win, attr)

// Print in center
#define wctrprintw(win, row, args...) {\
    char tmpstr[LIM_STR_BUFF];\
    sprintf(tmpstr, args);\
    mvwprintw(win, row, (win->_maxx - strlen(tmpstr))/2, "%s", tmpstr);\
}

// Function prototypes
int max(int n1, int n2);
int mod(int n1, int n2);
int rand_range(int min0, int max0);
time_t timestamp(void);
void msleep(time_t timer);
int max_strlen(const List_str strings, int min_len);
List_str dict_to_list(const str* obj, const int* ind, int len);
void sort_dict(Dict_str_int* dict);
WINDOW* new_win(int dim_y, int dim_x, int pos_y, int pos_x);
WINDOW* new_ctrwin(int dim_y, int dim_x);
void mv_win(WINDOW* win, int new_y, int new_x);
void unwin(WINDOW* win);
void quit(int err_code);