// Include libs
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "utils.h"

#define LEN_INTSTR 12
#define N_SPACES 2
#define FIRST_ALLOWED_CHAR '!'
#define LAST_ALLOWED_CHAR '~'

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

// Check file format
Dict_str_int check_conf_file(FILE* fptr, int lines, int lim) {
    int line, col, achar;
    Dict_str_int dict;
    alloc(str, dict.key, lines);
    for(line = 0; line < lines; line++) {
        alloc(char, dict.key[line], lim);
    }
    alloc(int, dict.val, lines);
    dict.len = lines;
    rewind(fptr);
    for(line = 0; line < lines; line++) { // For each line
        // Check string
        for(col = 0; col < lim; col++) {
            achar = getc(fptr);
            if(achar == EOF) {
                dict.len = -1; // ERROR in file
                return dict;
            }
            if(achar == ' ') {
                if(col == 0) {
                    dict.len = -1; // ERROR in file
                    return dict;
                }
                break;
            }
            if(!is_char_in((char)achar, FIRST_ALLOWED_CHAR, LAST_ALLOWED_CHAR)) {
                dict.len = -1; // ERROR in file
                return dict;
            }
            dict.key[line][col] = (char)achar;
        }
        dict.key[line][col] = '\0';

        // Check ' = '
        if((achar = getc(fptr)) != '=' || (achar = getc(fptr)) != ' ') {
            dict.len = -1; // ERROR in file
            return dict;
        }

        // Check value
        char numstr[LEN_INTSTR];
        for(col = 0; col < LEN_INTSTR; col++) {
            achar = getc(fptr);
            if(achar == EOF || achar == '\n') {
                if(col == 0) {
                    dict.len = -1; // ERROR in file
                    return dict;
                }
                break;
            }
            if(!is_char_in((char)achar, '0', '9')) {
                dict.len = -1; // ERROR in file
                return dict;
            }
            numstr[col] = (char)achar;
        }
        numstr[col] = '\0';
        dict.val[line] = atoi(numstr);
    }
    return dict;
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

void error(str err_str) {
    endwin();
    printf("%s\n", err_str);
    getchar();
    exit(1);
}