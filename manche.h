// Include libs
#include "sprites.h"

// Manche codes
#define MANCHE_LOST 0
#define MANCHE_RETR -1
#define MANCHE_CLOSE -2
#define MANCHE_QUIT -3

// Win codes
#define IN_GAME 0
#define LOST_GAME 1
#define WIN_GAME 2
#define HIGH_SCORE_GAME 3

// Frog constants
#define FROG_MOVE_Y 4
#define FROG_MOVE_X 2
#define FROG_INIT_Y LINE_BANK_2
#define FROG_INIT_X (MAIN_COLS/2-FROG_DIM_X/2)
#define FROG_LIM_UP LINE_HOLES
#define FROG_LIM_DOWN LINE_BANK_2
#define FROG_LIM_LEFT 0
#define FROG_LIM_RIGHT (MAIN_COLS-FROG_DIM_X)
#define FROG_NOT_ON_CROCCODILE (-1)
#define FROG_POSITION_SIG 0
#define FROG_SHOT_SIG 1

// Croccodile constants
#define MOVE_CROCCODILE_X 2
#define CROCCODILE_ID_INDEX 0
#define CROCCODILE_STREAM_INDEX 1
#define CROCCODILE_SPEED_INDEX 2
#define N_CROCCODILE_PARAMS 3
#define FREE_CROCCODILE (-1)
#define INCOMING_CROCCODILE (-2)
#define MIN_CROCCODILE_SPAWN_TIME 1
#define MAX_CROCCODILE_SPAWN_TIME 4
#define BAD_THRESHOLD DIFF_SET+2
#define BUBBLE_THRESHOLD 1000
#define GOOD_CROCCODILE_SIG 0
#define BAD_CROCCODILE_SIG 1
#define BUBBLE_CROCCODILE_SIG 2
#define IMMERSION_CROCCODILE_SIG 3

// Plant constants
#define INCOMING_PLANT (-1)
#define PLANT_ID_INDEX 0
#define PLANT_X_INDEX 1
#define N_PLANT_PARAMS 2
#define PLANT_SPAWN_SIG 0
#define PLANT_BULLET_SIG 1

// Bullet constants
#define FREE_BULLET (-1)
#define INCOMING_BULLET (-2)
#define BULLET_ID_INDEX 0
#define BULLET_Y_INDEX 1
#define BULLET_X_INDEX 2
#define N_BULLET_PARAMS 3
#define MOVE_BULLET_Y 1
#define BULLET_SPEED 10

// Function prototypes
Game_t play_manche(int score, int n_lifes, bool* holes_occupied);