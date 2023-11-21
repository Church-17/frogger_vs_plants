// Include libs
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

// Create new centred window
WINDOW* newctrwin(int n_rows, int n_cols) {
    return newwin(n_rows, n_cols, (LINES - (n_rows))/2, (COLS - (n_cols))/2);
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
        fprintf(stderr, "%s\n", errContainer[err_code-1]);
        getchar();
    }
    exit(err_code);
}