// Include libs
#include <curses.h>

// Define constant
#define ENTER 10

// Define macros

// - Math
#define mod(x, y) ((x%y + y) % y)

// - Print
#define wattrprintw(win, attr, fs, args...) wattron(win, attr); wprintw(win, fs, args); wattroff(win, attr) // Print string with attribute
#define mvwattrprintw(win, row, col, attr, fs, args...) wmove(win, row, col); wattrprintw(win, attr, fs, args) // Move & print string with attribute
#define wctrprintw(win, row, fs) mvwprintw(win, row, (win->_maxx - strlen(fs))/2, "%s", fs) // Print string centered
#define wctrattrprintw(win, row, attr, fs) wmvattrprintw(win, row, (win->_maxx - strlen(fs))/2, attr, "%s", fs) // Prints string centered with attribute

// - Delete window
#define unwin(win) werase(win); wrefresh(win); delwin(win); clear(); refresh()

// Define types and structs
typedef char* str;

typedef struct {
    int y;
    int x;
} Point;

typedef struct {
    str* list;
    int len;
} List_str;

// Define functions

// - Find max length of a strings list
int max_strlen(List_str strings, int min_len) {
    int max_len = min_len;
    for(int i = 0; i < strings.len; i++) {
        if(strlen(strings.list[i]) > max_len) {
            max_len = strlen(strings.list[i]);
        }
    }
    return max_len;
}