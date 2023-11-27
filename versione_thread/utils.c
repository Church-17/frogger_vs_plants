// Include libs
#include "utils.h"

// Max function
int max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

// Fixed mod function
int mod(int n1, int n2) {
    return n1 >= 0 ? n1%n2 : n1%n2+n2;
}

// Random number in range
int rand_range(int min0, int max0) {
    return rand() % (max0-min0) + min0;
}

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

void sort_dict(Dict_str_int* dict) {
    str tmpstr;
    int tmpint, ind_max = 0;
    for(int i = 0; i < dict->len - 1; i++) {
        for(int j = i; j < dict->len; j++) {
            if(dict->val[j] > dict->val[ind_max]) {
                ind_max = j;
            }
        }
        tmpstr = dict->key[i];
        dict->key[i] = dict->key[ind_max];
        dict->key[ind_max] = tmpstr;
        tmpint = dict->val[i];
        dict->val[i] = dict->val[ind_max];
        dict->val[ind_max] = tmpint;
    }
}

// Create new window
WINDOW* new_win(int dim_y, int dim_x, int pos_y, int pos_x) {
    WINDOW* new = newwin(dim_y, dim_x, pos_y, pos_x);
    if(new == NULL) {
        quit(ERR_WIN);
    }
    keypad(new, TRUE);
    return new;
}

// Create new window in center
WINDOW* new_ctrwin(int dim_y, int dim_x) {
    return new_win(dim_y, dim_x, LINES/2 - dim_y/2, COLS/2 - dim_x/2);
}

// Move window in central
void mv_win(WINDOW* win, int new_y, int new_x) {
    mvwin(win, new_y, new_x); // Move window
    clear(); // Remove garbage
    refresh();
}

// Delete a window
void unwin(WINDOW* win) {
    werase(win);
    wrefresh(win);
    delwin(win);
}

// Error handler
str errContainer[] = {
    "Current terminal doesn't support colors",
    "Error during memory allocation",
    "Error during window creation"
};
// Exit ending window with error message
void quit(int err_code) {
    endwin();
    if(err_code > 0) {
        fprintf(stderr, "%s\n", errContainer[err_code-1]);
        getchar();
    }
    exit(err_code);
}