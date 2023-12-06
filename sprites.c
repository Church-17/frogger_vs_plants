#include "manche.h"
#include "utils.h"
#include "res.h"
#include "struct.h"

void print_lifes(int n_lifes) {
    mvwprintw(main_scr, HEADER_ROW, LIFES_COL, "%*s", LIFES_SPACE, "");
    for(int i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + 3*i, RED_BLACK, "❤");
    }
}

void print_time(int time_remained, int new_timebar_len) {
    int i;
    attr_t time_color;
    if(time_remained < TIME_RED) {
        time_color = RED_BLACK;
    } else if(time_remained < TIME_YELLOW) {
        time_color = YELLOW_BLACK;
    } else {
        time_color = GREEN_BLACK;
    }
    mvwaprintw(main_scr, HEADER_ROW, TIME_COL, time_color, "%*d ", STRLEN_TIME, time_remained);
    for(i = 0; i < new_timebar_len; i++) {
        mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+i, time_color, "█");
    }
    for(i = new_timebar_len; i < TIMEBAR_LEN; i++) {
        mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+i, time_color, " ");
    }
}

void print_frog(WINDOW* win, int y, int x, int* colors_bg) {
    attr_t pair_bg[FROG_Y_DIM];
    for(int i = 0; i < FROG_Y_DIM; i++) {
        switch(colors_bg[i]) {
            case COLOR_BLUE:
                pair_bg[i] = GREEN_BLUE;
                break;

            case COLOR_PURPLE:
                pair_bg[i] = GREEN_PURPLE;
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
