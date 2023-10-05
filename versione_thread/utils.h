// Include libs
#include <curses.h>

// Define constant
#define ENTER 10

// Define macros
#define mod(x, y) ((x%y + y) % y)
#define wattrprintw(win, attr, fs, args...) wattron(win, attr); wprintw(win, fs, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, fs, args...) wmove(win, row, col); wattrprintw(win, attr, fs, args) // Move & print string with attribute
#define wctrprintw(win, row, fs) mvwprintw(win, row, (win->_maxx - strlen(fs))/2, "%s", fs) // Print string centered
#define wctrattrprintw(win, row, attr, fs) wmvattrprintw(win, row, (win->_maxx - strlen(fs))/2, attr, "%s", fs) // Prints string centered with attribute

// Define type and struct
typedef char* str;

typedef struct {
    int y;
    int x;
} Point;

typedef struct {
    str* list;
    int len;
} List_str;