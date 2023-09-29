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
Position scr_beg, scr_max;

// Prototypes
void init_scr();

// Main
int main(void) {

    init_scr(); // Initialize screen
    pthread_t t1, t2, t3; // Initilize threads


    endwin(); // End window
    return 0;
}

// Inizialize screen & fetch screen info
void init_scr() {
    initscr();
    noecho();
    curs_set(0); //Remove cursor
    getbegyx(stdscr, scr_beg.y, scr_beg.x); //Get screen dimension
    getmaxyx(stdscr, scr_max.y, scr_max.x);
}