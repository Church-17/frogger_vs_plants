// Include libs
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "utils.h"

#define LEN_INTSTR 12
#define N_SPACES 2

int max(int n1, int n2) {
    if(n1 > n2) {
        return n1;
    }
    return n2;
}

// Mod function
int mod(int n1, int n2) {
    if(n1 >= 0) {
        return n1 % n2;
    } else {
        return (n1 % n2) + n2;
    }
}

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

// Convert int to string
str dtos(int num) {
    dalloc(char, numstr, LEN_INTSTR);
    sprintf(numstr, "%d", num);
    return numstr;
}

// Check char
bool is_char_in(char ch, char first, char last) {
    if(ch >= first && ch <= last) {
        return 1;
    }
    return 0;
}

List_str dict_to_list(Dict_str_int dict) {
    List_str list;
    list.len = dict.len;
    alloc(str, list.list, list.len);
    for(int i = 0; i < dict.len; i++) {
        list.list[dict.val[i]] = dict.key[i];
    }
    return list;
}

// Create new centred window
WINDOW* newctrwin(int n_rows, int n_cols) {
    return newwin(n_rows, n_cols, (LINES - (n_rows))/2, (COLS - (n_cols))/2);
}

// Print in center
void wctrprintw(WINDOW* win, int row, str fstr) {
    mvwprintw(win, row, (win->_maxx - strlen(fstr) - N_SPACES)/2, " %s ", fstr);
}

// Print with attribute in center
void wctrattrprintw(WINDOW* win, int row, attr_t attr, str fstr) {
    mvwattrprintw(win, row, (win->_maxx - strlen(fstr) - N_SPACES)/2, attr, " %s ", fstr);
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
void error(str err_str) {
    endwin();
    printf("%s\n", err_str);
    getchar();
    exit(1);
}