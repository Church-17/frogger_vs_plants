// Include libs
#include <curses.h>
#include <string.h>
#include "utils.h"

// Find max length of a strings list
int max_strlen(List_str strings, int min_len) {
    int max_len = min_len;
    for(int i = 0; i < strings.len; i++) {
        if(strlen(strings.list[i]) > max_len) {
            max_len = strlen(strings.list[i]);
        }
    }
    return max_len;
}

str int_to_str(int num) {
    dalloc(char, numstr, (int)((ceil(log10(num))+1)*sizeof(char)));
    sprintf(numstr, "%d", num);
    return numstr;
}

// Print in center
void wctrprintw(WINDOW* win, int row, str fstr) {
    mvwprintw(win, row, (win->_maxx - strlen(fstr))/2, " %s ", fstr);
}

// Print with attribute in center
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr) {
    mvwattrprintw(win, row, (win->_maxx - strlen(fstr))/2, attr, " %s ", fstr);
}

// Print string with first letter attrihbuted
void wfattrprintw(WINDOW* win, attr_t attr, str fstr) {
    wattrprintw(win, attr, "%c", fstr[0]);
    wprintw(win, "%s", &(fstr[1]));
}

void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr) {
    mvwattrprintw(win, row, col, attr, "%c", fstr[0]);
    wprintw(win, "%s", &(fstr[1]));
}

// Delete a window
void unwin(WINDOW* win) {
    werase(win);
    wrefresh(win);
    delwin(win);
    clear();
    refresh();
}