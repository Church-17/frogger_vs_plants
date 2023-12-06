#include "manche.h"
#include "utils.h"
#include "res.h"
#include "struct.h"

void print_lifes(int n_lifes) {
    int i;
    static int old_lifes = N_LIFES;
    // mvwprintw(main_scr, HEADER_ROW, LIFES_COL, "%*s", LIFES_SPACE, "");
    for(i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "❤");
    }
    for(i = n_lifes; i < old_lifes; i++) {
        mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "%*s", SPACE_PER_LIFE, "");
    }
    old_lifes = n_lifes;
}

void print_time(int time_remained, int new_timebar_len) {
    int i;
    static int old_timebar_len = TIMEBAR_LEN;
    static attr_t time_color;
    if(time_remained == TIME_RED) {
        time_color = RED_BLACK;
        for(i = 0; i < new_timebar_len; i++) {
            mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+i, time_color, "█");
        }
    } else if(time_remained == TIME_YELLOW) {
        time_color = YELLOW_BLACK;
        for(i = 0; i < new_timebar_len; i++) {
            mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+i, time_color, "█");
        }
    } else if(time_remained == TIME_MANCHE) {
        time_color = GREEN_BLACK;
        for(i = 0; i < new_timebar_len; i++) {
            mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+i, time_color, "█");
        }
    }
    mvwaprintw(main_scr, HEADER_ROW, TIME_COL, time_color, "%*d ", STRLEN_TIME, time_remained);
    mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+new_timebar_len, WHITE_BLACK, "%*s", old_timebar_len-new_timebar_len, "");
    old_timebar_len = new_timebar_len;
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
