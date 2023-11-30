#include "sprites.h"
#include "utils.h"
#include "res.h"


void move_frog(WINDOW* win, int y, int x, List_attr cols_bg) {

    mvwaprintw(y, x, cols_bg.list[0], "▄█"); //
    mvwaprintw(y, x+3, MAGENTA_GREEN, "▀");
    mvwaprintw(y, x+4, GREEN_YELLOW, "▌▐");
    mvwaprintw(y, x+6, MAGENTA_GREEN, "▀");
    mvwaprintw(y, x+8, cols_bg.list[0], "█▄"); //

    mvwaprintw(y+1, x+1, cols_bg.list[1], "▀▄"); //
    mvwaprintw(y+1, x+3, GREEN_YELLOW, " ▄  ");
    mvwaprintw(y+1, x+7, cols_bg.list[1], "▄▀"); //

    mvwaprintw(y+2, x+2, cols_bg.list[2], "▄"); //
    mvwaprintw(y+2, x+3, GREEN_YELLOW, "█▄▀▄");
    mvwaprintw(y+2, x+7, cols_bg.list[2], "▄"); //

    mvwaprintw(y+3, x, cols_bg.list[3], "▀█▀"); //
    mvwaprintw(y+3, x+7, cols_bg.list[3], "▀█▀"); //
    
    return;
}
