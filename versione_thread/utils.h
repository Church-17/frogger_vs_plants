// Include libs
#include <curses.h>

// Define constant
#define ENTER 10

// Define macros
#define mod(x, y) ((x%y + y) % y)
#define wattrprintw(win, attr, str, args...) wattron(win, attr); wprintw(win, str, args); wattroff(win, attr) // prints a string enabling an attribute
#define mvwattrprintw(win, row, col, attr, str, args...) wmove(win, row, col); wattrprintw(win, attr, str, args) // printsa a string whit an attribute in a position
#define wctrprintw(win, row, str) mvwprintw(win, row, (win->_maxx - strlen(str))/2, "%s", str) // centers a string
#define wctrattrprintw(win, row, attr, str) wmvattrprintw(win, row, (win->_maxx - strlen(str))/2, attr, "%s", str) // prints a centered string with an attribute

// Define type and struct
typedef struct {
    int y;
    int x;
} Point;