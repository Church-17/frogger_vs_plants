// Include libs
#include "main.h"
#include "manche.h"
#include "str.h"
#include "utils.h"
#include "struct.h"

void print_background(const bool* holes_occupied) {
    // Print hedge
    for(int i = LINE_HEDGE; i < LINE_BANK_1; i++) {
        mvwaprintw(main_scr, i, 0, HEDGE_BG, "%*s", MAIN_COLS, "");
    }
    // Print holes
    for(int i = 0; i < N_HOLES; i++) {
        if(!holes_occupied[i]) { // If a hole is occupied, it's closed (so not printed)
            for(int j = LINE_HOLES; j < LINE_BANK_1; j++) {
                mvwaprintw(main_scr, j, i*MAIN_COLS/N_HOLES + (MAIN_COLS/N_HOLES - HOLE_DIM_X)/2, GREEN_PURPLE, "%*s", HOLE_DIM_X, "");
            }
        }
    }
    // Print upper bank
    for(int i = LINE_BANK_1; i < LINE_RIVER; i++) {
        mvwaprintw(main_scr, i, 0, BANK_BG, "%*s", MAIN_COLS, "");
    }
    // Print river
    for(int i = LINE_RIVER; i < LINE_BANK_2; i++) {
        mvwaprintw(main_scr, i, 0, RIVER_BG, "%*s", MAIN_COLS, "");
    }
    // Print lower bank
    for(int i = LINE_BANK_2; i < MAIN_ROWS; i++) {
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

void print_free_frog_bullet(int free_frog_bullet) {
    mvwaprintw(main_scr, LINE_HEADER, PRINT_BULLET_COL, WHITE_BLACK, "%s: %d/%d", STR_BULLETS, free_frog_bullet, MAX_BULLETS_PER_FROG);
}

void print_score(int score) {
    str diff[N_DIFFICULTY] = {STR_DIFFICULTY_0, STR_DIFFICULTY_1, STR_DIFFICULTY_2};
    attr_t score_color;
    if(DIFF_SET == 0) {
        score_color = GREEN_BLACK;
    } else if(DIFF_SET == 1) {
        score_color = YELLOW_BLACK;
    } else {
        score_color = RED_BLACK;
    }
    mvwaprintw(main_scr, LINE_HEADER, SCORE_COL, score_color, "%s: %*d  x%d (%s)", STR_SCORE, SCORE_LEN, score, SCORE_MULTIPLIER, diff[DIFF_SET]);
}

void print_lifes(int n_lifes) {
    for(int i = 0; i < n_lifes; i++) {
        mvwaprintw(main_scr, LINE_HEADER, MAIN_COLS - SPACE_PER_LIFE * (N_LIFES - i), RED_BLACK, "❤");
    }
    mvwaprintw(main_scr, LINE_HEADER, MAIN_COLS - SPACE_PER_LIFE * (N_LIFES - n_lifes), RED_BLACK, "%*s", SPACE_PER_LIFE*(N_LIFES-n_lifes), "");
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
    // Init vars
    int croccodile_stream, croccodile_id;
    attr_t restore_color_1, restore_color_2;
    attr_t pair_col[FROG_DIM_Y][FROG_DIM_X];
    static const str sprite_matrix[FROG_DIM_Y][FROG_DIM_X] = {
        {"▄", "█", " ", "▀", "▌", "▐", "▀", " ", "█", "▄"},
        {" ", "▀", "▄", " ", "▄", " ", " ", "▄", "▀", " "},
        {" ", " ", "▄", "█", "▄", "▀", "▄", "▄", " ", " "},
        {"▀", "█", "▀", " ", " ", " ", " ", "▀", "█", "▀"},
    };
    
    // Determine frog background
    if(gamevar->frog.y < LINE_RIVER || gamevar->frog.y >= LINE_BANK_2) {
        restore_color_1 = (SKIN_SET == 0) ? GREEN_PURPLE : RED_PURPLE;
    } else {
        if(gamevar->frog_on_croccodile >= 0) {
            croccodile_stream = (gamevar->frog.y - LINE_RIVER) / FROG_DIM_Y;
            croccodile_id = gamevar->frog_on_croccodile - MIN_CROCCODILE_ID - croccodile_stream*MAX_CROCCODILE_PER_STREAM;
            if(gamevar->croccodiles_kind[croccodile_stream][croccodile_id] == CROCCODILE_GOOD_SIG) {
                restore_color_1 = (SKIN_SET == 0) ? GREEN_DARKGREEN : RED_DARKGREEN; // If frog was on good croccodile set dark green
            } else {
                restore_color_1 = (SKIN_SET == 0) ? GREEN_BORDEAUX : RED_BORDEAUX; // If frog was on bad croccodile set bordeaux
            }
        } else {
            restore_color_1 = (SKIN_SET == 0) ? GREEN_DARKBLUE : RED_DARKBLUE;
        }
    }
    for(int i = 0; i < FROG_DIM_Y; i++) {
        for(int j = 0; j < 3; j++) {
            pair_col[i][j] = pair_col[i][FROG_DIM_X-1-j] = restore_color_1;
        }
    }
    for(int i = 3; i < 7; i++) {
        pair_col[3][i] = restore_color_1;
    }
    
    // Set fixed color
    if(SKIN_SET == 0) {
        restore_color_1 = GREEN_YELLOW;
        restore_color_2 = MAGENTA_GREEN;
    } else {
        restore_color_1 = RED_ORANGE;
        restore_color_2 = CYAN_RED;
    }
    pair_col[0][3] = pair_col[0][6] = restore_color_2;
    pair_col[0][4] = pair_col[0][5] = restore_color_1;
    for(int i = 1; i < 3; i++) {
        for(int j = 3; j < 7; j++) {
            pair_col[i][j] = restore_color_1;
        }
    }

    // Print frog
    for(int i = 0; i < FROG_DIM_Y; i++) {
        for(int j = 0; j < FROG_DIM_X; j++) {
            mvwaprintw(main_scr, gamevar->frog.y + i, gamevar->frog.x + j, pair_col[i][j], "%s", sprite_matrix[i][j]);
        }
    }
}

void print_croccodile(Position croccodile, int speed, int sig) {
    // Init vars
    int croccodile_beg, croccodile_len;
    attr_t croccodile_color_1, croccodile_color_2, croccodile_color_eye;
    attr_t pair_col[CROCCODILE_DIM_Y][CROCCODILE_DIM_X];
    static const str sprite_matrix[CROCCODILE_DIM_Y][CROCCODILE_DIM_X] = {
        {" ", " ", " ", " ", "▀", "▀", "▀", "█", "█", "█", "▄", "▄", "▄", "▄", "▄", "▄", "▄", "▄", "▄", "▄", "▄", "▄", "█", "█", "▀", "▀", "▀", " ", " ", " "},
        {"▄", "▄", "▄", "▄", " ", "▄", " ", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", " ", "▀", "█", "▄", "▄", "▄"},
        {"▀", "▀", "▀", "▀", " ", "▀", " ", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", "▄", "▀", " ", "▄", "█", "▀", "▀", "▀"},
        {" ", " ", " ", " ", "▄", "▄", "▄", "█", "█", "█", "▀", "▀", "▀", "▀", "▀", "▀", "▀", "▀", "▀", "▀", "▀", "▀", "█", "█", "▄", "▄", "▄", " ", " ", " "},
    };

    // Determine how much croccodile print
    if(croccodile.x < 0) {
        croccodile_beg = 0;
        croccodile_len = CROCCODILE_DIM_X + croccodile.x;
    } else if(croccodile.x < MAIN_COLS - CROCCODILE_DIM_X) {
        croccodile_beg = croccodile.x;
        croccodile_len = CROCCODILE_DIM_X;
    } else {
        croccodile_beg = croccodile.x;
        croccodile_len = MAIN_COLS - croccodile.x;
    }

    // Determine croccodile color
    if(sig == CROCCODILE_BAD_SIG || sig == CROCCODILE_BUBBLE_SIG) {
        croccodile_color_1 = BORDEAUX_DARKBLUE;
        croccodile_color_2 = PINK_BORDEAUX;
        croccodile_color_eye = WHITE_BORDEAUX;
    } else if(sig == CROCCODILE_GOOD_SIG) {
        croccodile_color_1 = DARKGREEN_DARKBLUE;
        croccodile_color_2 = LIGHTGREEN_DARKGREEN;
        croccodile_color_eye = WHITE_DARKGREEN;
    }
    for(int i = 0; i < CROCCODILE_DIM_X; i++) {
        pair_col[0][i] = pair_col[3][i] = croccodile_color_1;
    }
    for(int i = 0; i < 4; i++) {
        pair_col[1][i] = pair_col[1][CROCCODILE_DIM_X-1-i] = pair_col[2][i] = pair_col[2][CROCCODILE_DIM_X-1-i] = croccodile_color_1;
    }
    for(int i = 4; i < CROCCODILE_DIM_X-5; i++) {
        pair_col[1][i] = pair_col[2][i] = croccodile_color_2;
    }
    pair_col[1][CROCCODILE_DIM_X-5] = pair_col[2][CROCCODILE_DIM_X-5] = croccodile_color_eye;

    // Print croccodile if it isn't immersed
    if(sig != CROCCODILE_IMMERSION_SIG) { //
        if(speed > 0) {
            for(int i = 0; i < CROCCODILE_DIM_Y; i++) {
                for(int j = 0; j < croccodile_len; j++) {
                    mvwaprintw(main_scr, croccodile.y + i, j + croccodile_beg, pair_col[i][croccodile_beg-croccodile.x+j], "%s", sprite_matrix[i][croccodile_beg-croccodile.x+j]);
                }
            }
        } else {
            for(int i = 0; i < CROCCODILE_DIM_Y; i++) {
                for(int j = 0; j < croccodile_len; j++) {
                    mvwaprintw(main_scr, croccodile.y + i, j + croccodile_beg, pair_col[i][CROCCODILE_DIM_X-(croccodile_beg-croccodile.x+j)-1], "%s", sprite_matrix[i][CROCCODILE_DIM_X-(croccodile_beg-croccodile.x+j)-1]);
                }
            }
        }
    }

    // Print bubble
    if(sig == CROCCODILE_BUBBLE_SIG) {
        mvwaprintw(main_scr, croccodile.y, croccodile.x + 1, CYAN_DARKBLUE, "%s", "⬤");
        mvwaprintw(main_scr, croccodile.y, croccodile.x + CROCCODILE_DIM_X - 1, CYAN_DARKBLUE, "%s", "⬤");
    }
}

void print_plant(Position plant) {
    for(int i = 0; i < PLANT_DIM_Y; i++) {
        mvwaprintw(main_scr, plant.y + i, plant.x, MAGENTA_GREEN, "%*s", PLANT_DIM_X, "");
    }
}

void print_bullet(Position bullet) {
    for(int i = 0; i < BULLET_DIM_Y; i++) {
        mvwaprintw(main_scr, bullet.y + i, bullet.x, RED_BLACK | A_STANDOUT, "%*s", BULLET_DIM_X, "");
    }
}