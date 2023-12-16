// Include libs
#include "res.h"

// Define constant
#define MAIN_ROWS 47
#define MAIN_COLS 150
#define PAUSE_GAME_KEY 'p'
#define CLOSE_GAME_KEY 'q'
#define TIME_MANCHE 60
#define N_LIFES 6
#define N_HOLES 5
#define N_WATER_STREAM 8
#define MIN_STREAM_SPEED (2*DIFF_SET*DIFF_SET + 2)
#define MAX_STREAM_SPEED (MIN_STREAM_SPEED + 5)
#define N_PLANTS 3

// Define communication ID
#define RESIZE_ID (-3)
#define CLOSE_ID (-2)
#define PAUSE_ID (-1)
#define FROG_ID 0
#define TIME_ID 1
#define PLANT_1_ID 2
#define PLANT_2_ID 3
#define PLANT_3_ID 4
#define MIN_CROCCODILE_ID 5
#define MAX_CROCCODILE_PER_STREAM (5 - DIFF_SET)
#define MIN_BULLET_ID (N_WATER_STREAM*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID)

#define LIM_N_ENTITIES 100

// Define colors
#define HEDGE_BG (GREY_BLACK | A_STANDOUT)
#define BANK_BG GREEN_PURPLE
#define RIVER_BG GREEN_BLUE
#define GOOD_CROCCODILE_BG GREEN_DARKGREEN
#define BAD_CROCCODILE_BG GREEN_BOURDEAUX

// Function prototypes
void demo(void);
bool game(void);
void print_game(const Game_t* gamevar);