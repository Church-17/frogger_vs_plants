#include "manche.h"
#include "utils.h"
#include "res.h"
#include "struct.h"

void print_lifes(int n_lifes) {
    int i;
    for(i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "❤");
    }
    mvwaprintw(main_scr, HEADER_ROW, LIFES_COL + SPACE_PER_LIFE*n_lifes, RED_BLACK, "%*s", SPACE_PER_LIFE*(MAX_N_LIFES-n_lifes), "");
}

void print_time(int time_remained) {
    // Init vars
    int new_timebar_len = time_remained * ((double) TIMEBAR_LEN / TIME_MANCHE); // Calc timebar length
    attr_t time_color, timebar_color;

    // Determine color
    if(time_remained <= TIME_RED) {
        time_color = RED_BLACK;
        timebar_color = GREEN_RED;
    } else if(time_remained <= TIME_YELLOW) {
        time_color = YELLOW_BLACK;
        timebar_color = GREEN_YELLOW;
    } else if(time_remained <= TIME_MANCHE) {
        time_color = GREEN_BLACK;
        timebar_color = MAGENTA_GREEN;
    }

    // Print time & timebar
    mvwaprintw(main_scr, HEADER_ROW, TIME_COL, time_color, "%*d ", STRLEN_TIME, time_remained);
    mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL, timebar_color, "%*s", new_timebar_len, "");
    mvwaprintw(main_scr, HEADER_ROW, TIMEBAR_COL+new_timebar_len, time_color, "%*s", TIMEBAR_LEN-new_timebar_len, "");
}

void print_frog(const Game_t* gamevar) {
    int i;
    attr_t restore_color;
    attr_t pair_bg[FROG_Y_DIM];

    if (in_hole_line(gamevar->frog) && gamevar->frog.y < LINE_HOLES) {
        restore_color = GREEN_GREY;
    } else if(gamevar->frog.y < LINE_RIVER) {
        restore_color = GREEN_PURPLE;
    } else if(gamevar->frog.y < LINE_BANK_2) {
        restore_color = GREEN_BLUE;
    } else {
        restore_color = GREEN_PURPLE;
    }
    for(i = 0; i < FROG_Y_DIM; i++) {
        pair_bg[i] = restore_color;
    }

    mvwaprintw(main_scr, gamevar->frog.y, gamevar->frog.x, pair_bg[0], "▄█"); //
    mvwaprintw(main_scr, gamevar->frog.y, gamevar->frog.x+3, MAGENTA_GREEN, "▀");
    mvwaprintw(main_scr, gamevar->frog.y, gamevar->frog.x+4, GREEN_YELLOW, "▌▐");
    mvwaprintw(main_scr, gamevar->frog.y, gamevar->frog.x+6, MAGENTA_GREEN, "▀");
    mvwaprintw(main_scr, gamevar->frog.y, gamevar->frog.x+8, pair_bg[0], "█▄"); //

    mvwaprintw(main_scr, gamevar->frog.y+1, gamevar->frog.x+1, pair_bg[1], "▀▄"); //
    mvwaprintw(main_scr, gamevar->frog.y+1, gamevar->frog.x+3, GREEN_YELLOW, " ▄  ");
    mvwaprintw(main_scr, gamevar->frog.y+1, gamevar->frog.x+7, pair_bg[1], "▄▀"); //

    mvwaprintw(main_scr, gamevar->frog.y+2, gamevar->frog.x+2, pair_bg[2], "▄"); //
    mvwaprintw(main_scr, gamevar->frog.y+2, gamevar->frog.x+3, GREEN_YELLOW, "█▄▀▄");
    mvwaprintw(main_scr, gamevar->frog.y+2, gamevar->frog.x+7, pair_bg[2], "▄"); //

    mvwaprintw(main_scr, gamevar->frog.y+3, gamevar->frog.x, pair_bg[3], "▀█▀"); //
    mvwaprintw(main_scr, gamevar->frog.y+3, gamevar->frog.x+7, pair_bg[3], "▀█▀"); //
}

void print_holes(int num_hole, const bool* occupied) {
    int i, j;
    for (i = 0; i < N_HOLES; i++) {
        if (occupied[i]) {
            for (j = 1; j < HOLE_DIM_Y+1; j++) {
                mvwaprintw(main_scr, j, num_hole*HOLE_DIM_X + (num_hole*HOLE_DISTANCE), GREY_BLACK, "████████████"); 
            }
        } else {
            for (j = 1; j < HOLE_DIM_Y+1; j++) {
                mvwaprintw(main_scr, j, num_hole*HOLE_DIM_X + (num_hole*HOLE_DISTANCE), GREY_BLACK, "████████████"); 
            }
        }
    }
}
// Checks if the frog is in the line of a hole
bool in_hole_line(Position frog) {
    for (int i = 0; i < N_HOLES; i++) {
        if (frog.x >= i*HOLE_DIM_X + (i*HOLE_DISTANCE) && frog.x < i*HOLE_DIM_X + (i*HOLE_DISTANCE) + 3)
            return TRUE;
    }
    return FALSE;
}