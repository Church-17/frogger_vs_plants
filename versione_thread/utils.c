// Include libs
#include <sys/time.h>
#include "utils.h"
#include "struct.h"

// Max function
int max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

// Fixed mod function
int mod(int n1, int n2) {
    return n1 >= 0 ? n1%n2 : n1%n2+n2;
}

// Random number in range, min0 <= RAND < max0
int rand_range(int min0, int max0) {
    return rand() % (max0-min0) + min0;
}

// Get current epoch unix time in milliseconds
time_t timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL); // Get unix time in seconds and the microseconds
    time_t milliseconds = tv.tv_sec * MSEC_IN_SEC + tv.tv_usec / MSEC_IN_SEC; // Calc unix time in milliseconds
    return milliseconds;
}

// Find max length of a strings list, starting from a min
int max_strlen(const List_str strings, int min_len) {
    int max_len = min_len;
    for(int i = 0; i < strings.len; i++) {
        if(strlen(strings.list[i]) > max_len) {
            max_len = strlen(strings.list[i]);
        }
    }
    return max_len;
}

// Given objects and indexes, create a list with each object at its index
List_str dict_to_list(const str* obj, const int* ind, int len) {
    List_str list;
    list.len = len;
    alloc(str, list.list, list.len);
    for(int i = 0; i < len; i++) {
        list.list[ind[i]] = obj[i];
    }
    return list;
}

// Sort a dict descendent based on the value
void sort_dict(Dict_str_int* dict) {
    str tmpstr;
    int tmpint, ind_max;
    for(int i = 0; i < dict->len - 1; i++) {
        ind_max = i;
        for(int j = i+1; j < dict->len; j++) {
            if(dict->val[j] > dict->val[ind_max]) {
                ind_max = j; // Select index of the higher value
            }
        }
        // Swap the first non-ordered element with the selected element
        tmpstr = dict->key[i];
        dict->key[i] = dict->key[ind_max];
        dict->key[ind_max] = tmpstr;
        tmpint = dict->val[i];
        dict->val[i] = dict->val[ind_max];
        dict->val[ind_max] = tmpint;
    }
}

// Create new window with keypad
WINDOW* new_win(int dim_y, int dim_x, int pos_y, int pos_x) {
    WINDOW* new = newwin(dim_y, dim_x, pos_y, pos_x);
    if(new == NULL) {
        quit(ERR_WIN);
    }
    keypad(new, TRUE); // Enable keypad
    return new;
}

// Create new window in center
WINDOW* new_ctrwin(int dim_y, int dim_x) {
    return new_win(dim_y, dim_x, LINES/2 - dim_y/2, COLS/2 - dim_x/2);
}

// Delete a window
void un_win(WINDOW* win) {
    wclear(win);
    wrefresh(win);
    delwin(win);
}

// Error handler, exit ending window with error message
void quit(int err_code) {
    endwin();
    if(err_code > 0) {
        const str errContainer[] = {
            "Error allocating memory",
            "Current terminal doesn't support colors",
            "Error creating window",
            "Error playing music",
            "Error creating thread",
            "Error in semaphore wait",
            "Error in semaphore signal",
            "Error in mutex lock",
            "Error in mutex unlock",
            "Error in mutex condition",
            "Error in semaphore initialization",
            "Error in semaphore destroy",
        };
        perror(errContainer[err_code-1]);
    }
    exit(err_code);
}