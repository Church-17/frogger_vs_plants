#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <ncurses.h>

#include <locale.h>
#include <time.h>
#include "../struct.h"

#define FROG_X_DIM 10
#define FROG_Y_DIM 4

void move_frog(WINDOW* win, int y, int x);

#define mvaptrintw(y, x, attr, fstr...) attron(attr); mvprintw(y, x, fstr); attroff(attr)
#define GREEN_BLACK COLOR_PAIR(1)
#define GREEN_YELLOW COLOR_PAIR(2)
#define MAGENTA_GREEN COLOR_PAIR(3)
#define GREEN_RED COLOR_PAIR(5)
#define GREEN_BLUE COLOR_PAIR(4)

#define LIM_RED_BLUE 17

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
    init_pair(4, COLOR_GREEN, COLOR_BLUE);
    init_pair(5, COLOR_GREEN, COLOR_RED);

    int i, c, y = LINES/2, x = COLS/2, old_y, old_x;

    for(i = 0; i < LIM_RED_BLUE; i++) {
        mvaptrintw(i, 0, GREEN_RED, "%*c", COLS, ' ');
    }
    for(i = LIM_RED_BLUE; i < LINES; i++) {
        mvaptrintw(i, 0, GREEN_BLUE, "%*c", COLS, ' ');
    }

    move_frog(stdscr, y, x);

    while(1){
        old_y = y, old_x = x;
        c = getch();
        switch(c) {
            case KEY_UP:
                if(y > 1) y -=FROG_Y_DIM;
                if(y <= 1) y = 2;
                break;

            case KEY_DOWN:
                if(y < LINES-FROG_Y_DIM) y += FROG_Y_DIM;
                if(y >= LINES-FROG_Y_DIM) y = LINES-5;
                break;

            case KEY_LEFT:
                if(x > 1) x -= 2;
                if(x <= 1) x = 2;
                break;

            case KEY_RIGHT:
                if(x < COLS-FROG_X_DIM) x += 2;
                if(x >= COLS-FROG_X_DIM) x = COLS-11;
                break;

            case (int)'q':
                werase(stdscr);
                endwin();
                exit(0);

            default: continue;
        }
        for(i = old_y; i < old_y + FROG_Y_DIM; i++) {
            if(i < LIM_RED_BLUE) {
                mvaptrintw(i, old_x, GREEN_RED, "%*c", FROG_X_DIM, ' ');
            } else {
                mvaptrintw(i, old_x, GREEN_BLUE, "%*c", FROG_X_DIM, ' ');
            }
        }

        move_frog(stdscr, y, x);

    }

    return 0;
}

void move_frog(WINDOW* win, int y, int x) {
    int i;
    attr_t temp[FROG_Y_DIM];
    List_attr cols_bg;
    cols_bg.list = temp;
    cols_bg.len = FROG_Y_DIM;

    for(i = 0; i < FROG_Y_DIM; i++) {
        if(i + y < LIM_RED_BLUE) {
            cols_bg.list[i] = GREEN_RED;
        } else {
            cols_bg.list[i] = GREEN_BLUE;
        }
    }

    mvaptrintw(y, x, cols_bg.list[0], "▄█"); //
    mvaptrintw(y, x+3, MAGENTA_GREEN, "▀");
    mvaptrintw(y, x+4, GREEN_YELLOW, "▌▐");
    mvaptrintw(y, x+6, MAGENTA_GREEN, "▀");
    mvaptrintw(y, x+8, cols_bg.list[0], "█▄"); //

    mvaptrintw(y+1, x+1, cols_bg.list[1], "▀▄"); //
    mvaptrintw(y+1, x+3, GREEN_YELLOW, " ▄  ");
    mvaptrintw(y+1, x+7, cols_bg.list[1], "▄▀"); //

    mvaptrintw(y+2, x+2, cols_bg.list[2], "▄"); //
    mvaptrintw(y+2, x+3, GREEN_YELLOW, "█▄▀▄");
    mvaptrintw(y+2, x+7, cols_bg.list[2], "▄"); //

    mvaptrintw(y+3, x, cols_bg.list[3], "▀█▀"); //
    mvaptrintw(y+3, x+7, cols_bg.list[3], "▀█▀"); //
    
    return;
}