#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

int xBeg, yBeg, xMax, yMax;

typedef struct {
    int x;
    int y;
} Position;

int main(void) {

    initscr();
    noecho();
    curs_set(0);

    getbegyx(stdscr, yBeg, xBeg);
    getmaxyx(stdscr, yMax, xMax);

    pthread_t t1, t2, t3;


    return 0;
}