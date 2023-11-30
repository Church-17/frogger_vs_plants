#include <ncurses.h>
#include "sprites.h"
#include "utils.h"
#include "res.h"


void move_frog(WINDOW* win, int y, int x) {
    int i;
    attr_t temp[FROG_Y_DIM];
    List_attr cols_bg;
    cols_bg.list = temp;
    cols_bg.len = FROG_Y_DIM;

    // for(i = 0; i < FROG_Y_DIM; i++) { // eseguito da controllo() ..?
    //     if(i + y < LIM_RED_BLUE) {
    //         cols_bg.list[i] = GREEN_RED;
    //     } else {
    //         cols_bg.list[i] = GREEN_BLUE;
    //     }
    // }

    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_YELLOW);
    init_pair(3, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(4, COLOR_GREEN, COLOR_BLUE);
    init_pair(5, COLOR_GREEN, COLOR_RED);

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
