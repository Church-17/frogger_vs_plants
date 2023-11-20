// Include libs
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "utils.h"

// Define constant
#define N_SPACES_CTRPRINT 2

// Find max length of a strings list, starting from a min
int max_strlen(List_str strings, int min_len) {
    int max_len = min_len;
    for(int i = 0; i < strings.len; i++) {
        if(strlen(strings.list[i]) > max_len) {
            max_len = strlen(strings.list[i]);
        }
    }
    return max_len;
}

// Given objects and indexes, create a list with each object at its index
List_str dict_to_list(str* obj, int* ind, int len) {
    List_str list;
    list.len = len;
    alloc(str, list.list, list.len);
    for(int i = 0; i < len; i++) {
        list.list[ind[i]] = obj[i];
    }
    return list;
}

// Create new centred window
WINDOW* newctrwin(int n_rows, int n_cols) {
    return newwin(n_rows, n_cols, (LINES - (n_rows))/2, (COLS - (n_cols))/2);
}

// Print in center
void wctrprintw(WINDOW* win, int row, str fstr) {
    mvwprintw(win, row, (win->_maxx - strlen(fstr) - N_SPACES_CTRPRINT)/2, " %s ", fstr);
}

// Print with attribute in center
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr) {
    mvwattrprintw(win, row, (win->_maxx - strlen(fstr) - N_SPACES_CTRPRINT)/2, attr, " %s ", fstr);
}

// Print string with first letter attributed
void wfattrprintw(WINDOW* win, attr_t attr, str fstr) {
    wattrprintw(win, attr, "%c", fstr[0]);
    wprintw(win, "%s", &(fstr[1]));
}

// Move & print string with first letter attributed
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr) {
    mvwattrprintw(win, row, col, attr, "%c", fstr[0]);
    wprintw(win, "%s", &(fstr[1]));
}

// Delete a window
void unwin(WINDOW* win) {
    werase(win);
    wrefresh(win);
    delwin(win);
    refresh();
}

// Error handler
str errContainer[] = {
    "Current terminal doesn't support colors",
    "Error during memory allocation"
};
// Exit ending window with error message
void quit(int err_code) {
    endwin();
    if(err_code > 0) {
        printf("%s\n", errContainer[err_code-1]);
        getchar();
    }
    exit(err_code);
}