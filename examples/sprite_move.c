#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <ncurses.h>

#include <locale.h>
#include <time.h>
#include "../main.h"
#include "../menu.h"
#include "../game.h"
#include "../str.h"
#include "../utils.h"

#define FROG_X_DIM 10
#define FROG_Y_DIM 4

void move_frog(WINDOW* win, int y, int x);

#define mvaptrintw(y, x, attr, fstr...) attron(attr); mvprintw(y, x, fstr); attroff(attr)
#define GREEN_BLACK COLOR_PAIR(1)
#define GREEN_YELLOW COLOR_PAIR(2)
#define MAGENTA_GREEN COLOR_PAIR(3)




int main(void){
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    cbreak(); // Don't wait ENTER in getch()
    keypad(stdscr, TRUE); // Enable function keys listener

    start_color(); // Enable colors

    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_WHITE, 800, 800, 800);
    init_color(COLOR_RED, 800, 0, 0);
    init_color(COLOR_GREEN, 0, 700, 200);
    init_color(COLOR_YELLOW, 800, 800, 0);
    init_color(COLOR_BLUE, 0, 300, 900);
    init_color(COLOR_MAGENTA, 600, 0, 600);
    init_color(COLOR_CYAN, 200, 600, 900);
    // Set needed color pairs (text, bg)
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_YELLOW);
    init_pair(3, COLOR_MAGENTA, COLOR_GREEN);

    int y = LINES-5, x = COLS/2;
    move_frog(stdscr, y, x);

    while(1){

        int c = getch();
        switch (c) // controllo movimento
        {
            case KEY_UP: if (y > 1)  y -=3; if (y <= 1) y = 2; break;
            case KEY_DOWN: if (y < LINES-FROG_Y_DIM)  y += 3; if (y >= LINES-FROG_Y_DIM) y = LINES-5; break;
            case KEY_LEFT: if (x > 1)  x -= 3; if (x <= 1) x = 2; break;
            case KEY_RIGHT: if (x < COLS-FROG_X_DIM)  x += 3; if (x >= COLS-FROG_X_DIM) x = COLS-11; break;
            case (int)'q': werase(stdscr); endwin(); exit(0);
            default: continue;
        }
        move_frog(stdscr, y, x);

    }

    return 0;
}

void move_frog(WINDOW* win, int y, int x)
{
    if ((y > 1 && y < LINES-FROG_Y_DIM) && (x > 1 && x < COLS-FROG_X_DIM))
    {
        werase(win);
        wrefresh(win);

        mvaptrintw(y, x, GREEN_BLACK, "▄█"); //
        mvaptrintw(y, x+3, MAGENTA_GREEN, "▀");
        mvaptrintw(y, x+4, GREEN_YELLOW, "▌▐");
        mvaptrintw(y, x+6, MAGENTA_GREEN, "▀");
        mvaptrintw(y, x+8, GREEN_BLACK, "█▄"); //

        mvaptrintw(y+1, x+1, GREEN_BLACK, "▀▄"); //
        mvaptrintw(y+1, x+3, GREEN_YELLOW, " ▄  ");
        mvaptrintw(y+1, x+7, GREEN_BLACK, "▄▀"); //

        mvaptrintw(y+2, x+2, GREEN_BLACK, "▄"); //
        mvaptrintw(y+2, x+3, GREEN_YELLOW, "█▄▀▄");
        mvaptrintw(y+2, x+7, GREEN_BLACK, "▄"); //

        mvaptrintw(y+3, x, GREEN_BLACK, "▀█▀"); //
        mvaptrintw(y+3, x+7, GREEN_BLACK, "▀█▀"); //
    }
    return;
}