// Include libs
#include "sprites.h"
#include "str.h"
#include "utils.h"
#include "struct.h"

void print_background(const bool* holes_occupied) {
    int i, j;
    // Print hedge
    for(i = LINE_HEDGE; i < LINE_BANK_1; i++) {
        mvwaprintw(main_scr, i, 0, HEDGE_BG, "%*s", MAIN_COLS, "");
    }
    // Print holes
    for(i = 0; i < N_HOLES; i++) {
        if(!holes_occupied[i]) { // If a hole is occupied, it's closed (so not printed)
            for(j = LINE_HOLES; j < LINE_BANK_1; j++) {
                mvwaprintw(main_scr, j, i*MAIN_COLS/N_HOLES + HOLE_PAD_X, GREEN_PURPLE, "%*s", HOLE_DIM_X, "");
            }
        }
    }
    // Print upper bank
    for(i = LINE_BANK_1; i < LINE_RIVER; i++) {
        mvwaprintw(main_scr, i, 0, BANK_BG, "%*s", MAIN_COLS, "");
    }
    // Print river
    for(i = LINE_RIVER; i < LINE_BANK_2; i++) {
        mvwaprintw(main_scr, i, 0, RIVER_BG, "%*s", MAIN_COLS, "");
    }
    // Print lower bank
    for(i = LINE_BANK_2; i < MAIN_ROWS; i++) {
        mvwaprintw(main_scr, i, 0, BANK_BG, "%*s", MAIN_COLS, "");
    }
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
    mvwaprintw(main_scr, LINE_HEADER, TIME_COL, time_color, "%*d ", STRLEN_TIME, time_remained);
    mvwaprintw(main_scr, LINE_HEADER, TIMEBAR_COL, timebar_color, "%*s", new_timebar_len, "");
    mvwaprintw(main_scr, LINE_HEADER, TIMEBAR_COL+new_timebar_len, time_color, "%*s", TIMEBAR_LEN-new_timebar_len, "");
}

void print_score(int score) {
    mvwaprintw(main_scr, LINE_HEADER, SCORE_COL, WHITE_BLACK, "%s: %d", STR_SCORE, score);
}

void print_lifes(int n_lifes) {
    int i;
    for(i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, LINE_HEADER, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "❤");
    }
    mvwaprintw(main_scr, LINE_HEADER, LIFES_COL + SPACE_PER_LIFE*n_lifes, RED_BLACK, "%*s", SPACE_PER_LIFE*(N_LIFES-n_lifes), "");
}

void print_frog(const Game_t* gamevar) {
    int i, j;
    attr_t restore_color;
    attr_t pair_col[FROG_DIM_Y][FROG_DIM_X];

    static const str sprite_matrix[FROG_DIM_Y][FROG_DIM_X] = {
        {"▄", "█", " ", "▀", "▌", "▐", "▀", " ", "█", "▄"},
        {" ", "▀", "▄", " ", "▄", " ", " ", "▄", "▀", " "},
        {" ", " ", "▄", "█", "▄", "▀", "▄", "▄", " ", " "},
        {"▀", "█", "▀", " ", " ", " ", " ", "▀", "█", "▀"},
    };
    
    // Determine frog background
    if(gamevar->frog.y < LINE_RIVER) {
        restore_color = BANK_BG;
    } else if(gamevar->frog.y < LINE_BANK_2) {
        restore_color = GOOD_CROCCODILE_BG;
    } else {
        restore_color = BANK_BG;
    }
    for(i = 0; i < FROG_DIM_Y; i++) {
        for(j = 0; j < FROG_DIM_X; j++) {
            pair_col[i][j] = restore_color;
        }
    }
    
    // Set fixed color
    for(i = 0; i < FROG_DIM_Y-1; i++) {
        for(j = 3; j < 7; j++) {
            pair_col[i][j] = GREEN_YELLOW;
        }
    }
    pair_col[0][3] = pair_col[0][6] = MAGENTA_GREEN;

    // Print frog
    for(i = 0; i < FROG_DIM_Y; i++) {
        for(j = 0; j < FROG_DIM_X; j++) {
            mvwaprintw(main_scr, gamevar->frog.y + i, gamevar->frog.x + j, pair_col[i][j], "%s", sprite_matrix[i][j]);
        }
    }
}

void print_croccodile(Position croccodile, bool direction) {
    int i;
    if(croccodile.x < 0) {
        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
            mvwaprintw(main_scr, i + croccodile.y, 0, GREEN_DARKGREEN, "%*s", CROCCODILE_DIM_X + croccodile.x, "");
        }
    } else if(croccodile.x < MAIN_COLS - CROCCODILE_DIM_X) {
        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
            mvwaprintw(main_scr, i + croccodile.y, croccodile.x, GREEN_DARKGREEN, "%*s", CROCCODILE_DIM_X, "");
        }
    } else {
        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
            mvwaprintw(main_scr, i + croccodile.y, croccodile.x, GREEN_DARKGREEN, "%*s", MAIN_COLS - croccodile.x, "");
        }
    }
}
