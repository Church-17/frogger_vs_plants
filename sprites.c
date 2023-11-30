#include "sprites.h"
#include "utils.h"
#include "res.h"
#include "struct.h"

void print_frog(WINDOW* win, int y, int x, int* colors_bg) {
    attr_t pair_bg[FROG_Y_DIM];
    for(int i = 0; i < FROG_Y_DIM; i++) {
        switch(colors_bg[i]) {
            case COLOR_BLACK:
                pair_bg[i] = GREEN_BLACK;
                break;
        }
    }

    mvwaprintw(win, y, x, pair_bg[0], "▄█"); //
    mvwaprintw(win, y, x+3, MAGENTA_GREEN, "▀");
    mvwaprintw(win, y, x+4, GREEN_YELLOW, "▌▐");
    mvwaprintw(win, y, x+6, MAGENTA_GREEN, "▀");
    mvwaprintw(win, y, x+8, pair_bg[0], "█▄"); //

    mvwaprintw(win, y+1, x+1, pair_bg[1], "▀▄"); //
    mvwaprintw(win, y+1, x+3, GREEN_YELLOW, " ▄  ");
    mvwaprintw(win, y+1, x+7, pair_bg[1], "▄▀"); //

    mvwaprintw(win, y+2, x+2, pair_bg[2], "▄"); //
    mvwaprintw(win, y+2, x+3, GREEN_YELLOW, "█▄▀▄");
    mvwaprintw(win, y+2, x+7, pair_bg[2], "▄"); //

    mvwaprintw(win, y+3, x, pair_bg[3], "▀█▀"); //
    mvwaprintw(win, y+3, x+7, pair_bg[3], "▀█▀"); //
    
    return;
}
