#include "manche.h"
#include "utils.h"
#include "res.h"
#include "struct.h"

void print_lifes(int n_lifes) {
    int i;
    static int old_lifes = N_LIFES;
    for(i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "❤");
    }
    for(i = n_lifes; i < old_lifes; i++) {
        mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "%*s", SPACE_PER_LIFE, "");
    }
    old_lifes = n_lifes;
}

void print_time(int time_remained, int new_timebar_len, bool reprint_bar) {
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
    } else if(reprint_bar) {
        for(i = 0; i < new_timebar_len; i++) {
            mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+i, time_color, "█");
        }
    }
    mvwaprintw(main_scr, HEADER_ROW, TIME_COL, time_color, "%*d ", STRLEN_TIME, time_remained);
    mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+new_timebar_len, WHITE_BLACK, "%*s", old_timebar_len-new_timebar_len, "");
    old_timebar_len = new_timebar_len;
}

void print_frog(Position frog, int* colors_bg) {
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

    mvwaprintw(main_scr, frog.y, frog.x, pair_bg[0], "▄█"); //
    mvwaprintw(main_scr, frog.y, frog.x+3, MAGENTA_GREEN, "▀");
    mvwaprintw(main_scr, frog.y, frog.x+4, GREEN_YELLOW, "▌▐");
    mvwaprintw(main_scr, frog.y, frog.x+6, MAGENTA_GREEN, "▀");
    mvwaprintw(main_scr, frog.y, frog.x+8, pair_bg[0], "█▄"); //

    mvwaprintw(main_scr, frog.y+1, frog.x+1, pair_bg[1], "▀▄"); //
    mvwaprintw(main_scr, frog.y+1, frog.x+3, GREEN_YELLOW, " ▄  ");
    mvwaprintw(main_scr, frog.y+1, frog.x+7, pair_bg[1], "▄▀"); //

    mvwaprintw(main_scr, frog.y+2, frog.x+2, pair_bg[2], "▄"); //
    mvwaprintw(main_scr, frog.y+2, frog.x+3, GREEN_YELLOW, "█▄▀▄");
    mvwaprintw(main_scr, frog.y+2, frog.x+7, pair_bg[2], "▄"); //

    mvwaprintw(main_scr, frog.y+3, frog.x, pair_bg[3], "▀█▀"); //
    mvwaprintw(main_scr, frog.y+3, frog.x+7, pair_bg[3], "▀█▀"); //
}

void print_game(Game_t gamevar) {
    // Print background
    print_background();

    // Print frog
    int frog_restore_colors[FROG_Y_DIM] = {COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE};
    print_frog(gamevar.frog, frog_restore_colors);

    // Print time
    double char_in_sec_timebar = (double) TIMEBAR_LEN / TIME_MANCHE;
    print_time(gamevar.timer, gamevar.timer * char_in_sec_timebar, TRUE);

    // Print lifes
    print_lifes(*(gamevar.lifes));
}
