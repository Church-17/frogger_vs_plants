// Include libs
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
//#include "struct.h"
#include "menu.h"

// Define constant

// Prototypes

// Main
int main(void) {
    Point scr_beg, scr_max;
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); //Remove cursor
    getbegyx(stdscr, scr_beg.y, scr_beg.x);
    getmaxyx(stdscr, scr_max.y, scr_max.x); //Get screen dimension

    pthread_t t1, t2, t3; // Initilize threads

    int chosen = home_menu(scr_max); // Call Home Menu
    printw("Your choiche is %d\n", chosen);
    
    getch();
    endwin(); // End window
    return 0;
}