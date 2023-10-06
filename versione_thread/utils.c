// Include libs
#include <curses.h>
#include <string.h>
#include "utils.h"
#include "struct.h"

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