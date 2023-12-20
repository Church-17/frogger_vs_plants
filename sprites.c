// Include libs
#include "main.h"
#include "sprites.h"
#include "str.h"
#include "utils.h"
#include "struct.h"
#include "manche.h" 

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
    attr_t time_color;

    // Determine color
    if(time_remained <= TIME_RED) {
        time_color = RED_BLACK;
    } else if(time_remained <= TIME_YELLOW) {
        time_color = YELLOW_BLACK;
    } else if(time_remained <= TIME_MANCHE) {
        time_color = GREEN_BLACK;
    }

    // Print time & timebar
    mvwaprintw(main_scr, LINE_HEADER, TIME_COL, time_color, "%*d ", STRLEN_TIME, time_remained);
    mvwaprintw(main_scr, LINE_HEADER, TIMEBAR_COL, time_color | A_STANDOUT, "%*s", new_timebar_len, "");
    mvwaprintw(main_scr, LINE_HEADER, TIMEBAR_COL+new_timebar_len, time_color, "%*s", TIMEBAR_LEN-new_timebar_len, "");
}

void print_score(int score) {
    str diff[N_DIFFICULTY] = {STR_DIFFICULTY_0, STR_DIFFICULTY_1, STR_DIFFICULTY_2};
    mvwaprintw(main_scr, LINE_HEADER, SCORE_COL, WHITE_BLACK, "%s: %s ( x%d ) | %s: %*d", STR_DIFFICULTY, diff[DIFF_SET], DIFF_SET+1, STR_SCORE, SCORE_LEN, score);
}

void print_lifes(int n_lifes) {
    int i;
    for(i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, LINE_HEADER, LIFES_COL + SPACE_PER_LIFE*i, RED_BLACK, "❤");
    }
    mvwaprintw(main_scr, LINE_HEADER, LIFES_COL + SPACE_PER_LIFE*n_lifes, RED_BLACK, "%*s", SPACE_PER_LIFE*(N_LIFES-n_lifes), "");
}

void print_figlet(int win) {
    wattron(main_scr, FIGLET_COLOR);
    if(win == LOST_GAME) {
        mvwprintw(main_scr, LINE_FIGLET + 0, LOSE_FIGLET_X, "██╗   ██╗ ██████╗ ██╗   ██╗    ██╗      ██████╗ ███████╗███████╗");
        mvwprintw(main_scr, LINE_FIGLET + 1, LOSE_FIGLET_X, "╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║     ██╔═══██╗██╔════╝██╔════╝");
        mvwprintw(main_scr, LINE_FIGLET + 2, LOSE_FIGLET_X, " ╚████╔╝ ██║   ██║██║   ██║    ██║     ██║   ██║███████╗█████╗  ");
        mvwprintw(main_scr, LINE_FIGLET + 3, LOSE_FIGLET_X, "  ╚██╔╝  ██║   ██║██║   ██║    ██║     ██║   ██║╚════██║██╔══╝  ");
        mvwprintw(main_scr, LINE_FIGLET + 4, LOSE_FIGLET_X, "   ██║   ╚██████╔╝╚██████╔╝    ███████╗╚██████╔╝███████║███████╗");
        mvwprintw(main_scr, LINE_FIGLET + 5, LOSE_FIGLET_X, "   ╚═╝    ╚═════╝  ╚═════╝     ╚══════╝ ╚═════╝ ╚══════╝╚══════╝");
    } else if(win == WIN_GAME) {
        mvwprintw(main_scr, LINE_FIGLET + 0, WIN_FIGLET_X, "██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗");
        mvwprintw(main_scr, LINE_FIGLET + 1, WIN_FIGLET_X, "╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║");
        mvwprintw(main_scr, LINE_FIGLET + 2, WIN_FIGLET_X, " ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║");
        mvwprintw(main_scr, LINE_FIGLET + 3, WIN_FIGLET_X, "  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║");
        mvwprintw(main_scr, LINE_FIGLET + 4, WIN_FIGLET_X, "   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║");
        mvwprintw(main_scr, LINE_FIGLET + 5, WIN_FIGLET_X, "   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝");
    } else if(win == HIGH_SCORE_GAME) {
        mvwprintw(main_scr, LINE_FIGLET + 0, BEST_FIGLET_X, "███╗   ██╗███████╗██╗    ██╗    ██╗  ██╗██╗ ██████╗ ██╗  ██╗    ███████╗ ██████╗ ██████╗ ██████╗ ███████╗");
        mvwprintw(main_scr, LINE_FIGLET + 1, BEST_FIGLET_X, "████╗  ██║██╔════╝██║    ██║    ██║  ██║██║██╔════╝ ██║  ██║    ██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔════╝");
        mvwprintw(main_scr, LINE_FIGLET + 2, BEST_FIGLET_X, "██╔██╗ ██║█████╗  ██║ █╗ ██║    ███████║██║██║  ███╗███████║    ███████╗██║     ██║   ██║██████╔╝█████╗  ");
        mvwprintw(main_scr, LINE_FIGLET + 3, BEST_FIGLET_X, "██║╚██╗██║██╔══╝  ██║███╗██║    ██╔══██║██║██║   ██║██╔══██║    ╚════██║██║     ██║   ██║██╔══██╗██╔══╝  ");
        mvwprintw(main_scr, LINE_FIGLET + 4, BEST_FIGLET_X, "██║ ╚████║███████╗╚███╔███╔╝    ██║  ██║██║╚██████╔╝██║  ██║    ███████║╚██████╗╚██████╔╝██║  ██║███████╗");
        mvwprintw(main_scr, LINE_FIGLET + 5, BEST_FIGLET_X, "╚═╝  ╚═══╝╚══════╝ ╚══╝╚══╝     ╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═╝    ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝");
    }
    wattroff(main_scr, FIGLET_COLOR);
}

void print_frog(const Game_t* gamevar) {
    int i, j, croccodile_stream, croccodile_id;
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
        if(gamevar->frog_on_croccodile >= 0) {
            croccodile_stream = (gamevar->frog.y - LINE_RIVER) / FROG_DIM_Y;
            croccodile_id = gamevar->frog_on_croccodile - MIN_CROCCODILE_ID - croccodile_stream*MAX_CROCCODILE_PER_STREAM;
            if(gamevar->bad_croccodiles[croccodile_stream][croccodile_id]) {
                restore_color = BAD_CROCCODILE_BG; // If frog was on bad croccodile set bordeaux
            } else {
                restore_color = GOOD_CROCCODILE_BG; // If frog was on good croccodile set dark green
            }
        } else {
            restore_color = RIVER_BG;
        }
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

void print_croccodile(Position croccodile, bool direction, bool is_bad) {
    int i;
    if(is_bad) {
        if(croccodile.x < 0) {
            for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                mvwaprintw(main_scr, i + croccodile.y, 0, BAD_CROCCODILE_BG, "%*s", CROCCODILE_DIM_X + croccodile.x, "");
            }
        } else if(croccodile.x < MAIN_COLS - CROCCODILE_DIM_X) {
            for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                mvwaprintw(main_scr, i + croccodile.y, croccodile.x, BAD_CROCCODILE_BG, "%*s", CROCCODILE_DIM_X, "");
            }
        } else {
            for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                mvwaprintw(main_scr, i + croccodile.y, croccodile.x, BAD_CROCCODILE_BG, "%*s", MAIN_COLS - croccodile.x, "");
            }
        }
    } else {
        if(croccodile.x < 0) {
            for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                mvwaprintw(main_scr, i + croccodile.y, 0, GOOD_CROCCODILE_BG, "%*s", CROCCODILE_DIM_X + croccodile.x, "");
            }
        } else if(croccodile.x < MAIN_COLS - CROCCODILE_DIM_X) {
            for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                mvwaprintw(main_scr, i + croccodile.y, croccodile.x, GOOD_CROCCODILE_BG, "%*s", CROCCODILE_DIM_X, "");
            }
        } else {
            for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                mvwaprintw(main_scr, i + croccodile.y, croccodile.x, GOOD_CROCCODILE_BG, "%*s", MAIN_COLS - croccodile.x, "");
            }
        }
    }
}
