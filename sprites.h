// Include libs
#include "game.h"

// Define game zones
#define LINE_HEADER 0
#define LINE_HEDGE 1
#define LINE_HOLES 2
#define LINE_BANK_1 (LINE_HOLES + FROG_DIM_Y)
#define LINE_RIVER (LINE_BANK_1 + FROG_DIM_Y)
#define LINE_BANK_2 (LINE_RIVER + N_WATER_STREAM*FROG_DIM_Y)

// Define colors
#define FIGLET_COLOR GREEN_DARKBLUE
#define HEDGE_BG (GREY_BLACK | A_STANDOUT)
#define BANK_BG GREEN_PURPLE
#define RIVER_BG GREEN_DARKBLUE
#define GOOD_CROCCODILE_BG GREEN_DARKGREEN
#define BAD_CROCCODILE_BG GREEN_BORDEAUX

// Figlet constant
#define LINE_FIGLET (MAIN_ROWS - 15)
#define TITLE_FIGLET_X 7
#define LOSE_FIGLET_X 42
#define WIN_FIGLET_X 47
#define BEST_FIGLET_X 22

// Time constants
#define TIME_COL 0
#define TIME_YELLOW 30
#define TIME_RED 10
#define TIMEBAR_LEN 30
#define STRLEN_TIME 3
#define TIMEBAR_COL (TIME_COL+4)

// Score constant
#define SCORE_COL 75
#define SCORE_LEN 3

// Lifes constants
#define SPACE_PER_LIFE 3
#define LIFES_COL (MAIN_COLS - SPACE_PER_LIFE * N_LIFES)

// Hole
#define HOLE_DIM_X (FROG_DIM_X + 2)
#define HOLE_DIM_Y FROG_DIM_Y
#define HOLE_PAD_X ((MAIN_COLS/N_HOLES - HOLE_DIM_X)/2)

// Frog constant
#define FROG_DIM_X 10
#define FROG_DIM_Y 4

// Croccodile constants
#define CROCCODILE_DIM_X 30
#define CROCCODILE_DIM_Y 4
#define MOVE_CROCCODILE_X 2

// Function prototypes
void print_background(const bool* holes_occupied);
void print_time(int time_remained);
void print_score(int score);
void print_lifes(int n_lifes);
void print_figlet(int win);
void print_frog(const Game_t* gamevar);
void print_croccodile(Position croccodile, bool direction, bool good);