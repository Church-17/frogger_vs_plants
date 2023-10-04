// Include libs
#include <curses.h>

// Define constant
#define ENTER 10

// Define macros
#define mod(x, y) ((x%y + y) % y)
#define wattrprintw(win, attr, str, args...) wattron(win, attr); wprintw(win, str, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, str, args...) wmove(win, row, col); wattrprintw(win, attr, str, args) // Move & print string with attribute
#define wctrprintw(win, row, str) mvwprintw(win, row, (win->_maxx - strlen(str))/2, "%s", str) // Print string centered
#define wctrattrprintw(win, row, attr, str) wmvattrprintw(win, row, (win->_maxx - strlen(str))/2, attr, "%s", str) // Prints string centered with attribute

// Define type and struct
typedef struct {
    int y;
    int x;
} Point;