// Include libs
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

// Define constant

// Define type and struct
typedef struct {
    int y;
    int x;
} Position;

// Define global vars

// Prototypes

// Main
int main(void) {
    Position scr_beg, scr_max;
    initscr(); // Initialize window
    noecho();
    curs_set(0); //Remove cursor
    getbegyx(stdscr, scr_beg.y, scr_beg.x);
    getmaxyx(stdscr, scr_max.y, scr_max.x); //Get screen dimension

    pthread_t t1, t2, t3; // Initilize threads


    endwin(); // End window
    return 0;
}