// Include libs
#include <stdlib.h>
#include <curses.h>

// Define constant
#define ENTER 10

// Define macros
#define mod(x, y) ((x%y + y) % y)
#define wattrprintw(win, attr, str, args...) wattron(win, attr); wprintw(win, str, args); wattroff(win, attr)
#define wmvattrprintw(win, row, col, attr, str, args...) wmove(win, row, col); wattrprintw(win, attr, str, args)
#define wctrprintw(win, row, str) mvwprintw(win, row, (win->_maxx - strlen(str))/2, "%s", str) 
#define wctrattrprintw(win, row, attr, str) wmvattrprintw(win, row, (win->_maxx - strlen(str))/2, attr, "%s", str) 

// Define type and struct
typedef struct {
    int y;
    int x;
} Point;