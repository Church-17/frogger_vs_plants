// Include libs
#include "res.h"

// Define constants
#define MAIN_ROWS 47
#define MAIN_COLS 150
#define PAUSE_GAME_KEY 'p'
#define CLOSE_GAME_KEY 'q'
#define SHOT_GAME_KEY ' '
#define MOVE_N_KEY 'w'
#define MOVE_W_KEY 'a'
#define MOVE_S_KEY 's'
#define MOVE_E_KEY 'd'
#define TIME_MANCHE 60
#define N_LIFES 5
#define N_HOLES 1
#define N_MANCHES N_HOLES
#define N_WATER_STREAM 1
#define N_PLANTS 3
#define MIN_STREAM_SPEED (4*DIFF_SET*DIFF_SET + 4)
#define MAX_STREAM_SPEED (MIN_STREAM_SPEED + 10)
#define SCORE_MULTIPLIER (DIFF_SET + 1)

// Define communication ID
#define RESIZE_ID (-3)
#define CLOSE_ID (-2)
#define PAUSE_ID (-1)
#define FROG_ID 0
#define TIME_ID 1
#define MIN_CROCCODILE_ID 2
#define MAX_CROCCODILE_PER_STREAM 5
#define MIN_PLANT_ID (N_WATER_STREAM*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID)
#define MIN_FROG_BULLET_ID (N_PLANTS + MIN_PLANT_ID)
#define MAX_BULLETS_PER_FROG 3
#define MIN_PLANT_BULLET_ID (MAX_BULLETS_PER_FROG + MIN_FROG_BULLET_ID)
#define MAX_BULLETS_PER_PLANT 5
#define LIM_N_ENTITIES (N_PLANTS*MAX_BULLETS_PER_PLANT + MIN_PLANT_BULLET_ID)

// Win codes
#define IN_GAME 0
#define LOST_GAME 1
#define WIN_GAME 2
#define HIGH_SCORE_GAME 3

// Function prototypes
void demo(void);
bool game(void);
void print_game(const Game_t* gamevar);