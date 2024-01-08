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

// Figlet constants
#define LINE_FIGLET (MAIN_ROWS - 15)
#define TITLE_FIGLET_X 7
#define LOSE_FIGLET_X 42
#define WIN_FIGLET_X 47
#define BEST_FIGLET_X 22

// Time constants
#define TIME_COL 1
#define TIME_YELLOW 30
#define TIME_RED 10
#define TIMEBAR_LEN 60
#define STRLEN_TIME 3
#define TIMEBAR_COL (TIME_COL+4)

// Score constants
#define SCORE_COL 100
#define SCORE_LEN 3

// Lifes constants
#define SPACE_PER_LIFE 3

// Hole constants
#define HOLE_DIM_X (FROG_DIM_X + 2)
#define HOLE_DIM_Y FROG_DIM_Y

// Frog constants
#define FROG_DIM_X 10
#define FROG_DIM_Y 4

// Croccodile constants
#define CROCCODILE_DIM_X 30
#define CROCCODILE_DIM_Y 4

// Plant constants
#define PLANT_DIM_X 9
#define PLANT_DIM_Y 4

// Bullet constans
#define BULLET_DIM_X 1
#define BULLET_DIM_Y 1
#define PRINT_BULLET_COL 80

// Function prototypes
void print_background(const bool* holes_occupied);
void print_time(int time_remained);
void print_free_frog_bullet(int free_frog_bullet);
void print_score(int score);
void print_lifes(int n_lifes);
void print_figlet(int win);
void print_frog(const Game_t* gamevar);
void print_croccodile(Position croccodile, int speed, int sig);
void print_plant(Position plant);
void print_bullet(Position bullet);