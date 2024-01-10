// Include libs
#include "sprites.h"

// Manche codes
#define MANCHE_LOST 0
#define MANCHE_RETR -1
#define MANCHE_CLOSE -2
#define MANCHE_QUIT -3

// Entity constant
#define N_ENTITY_PARAMS 3
#define FREE_ENTITY (-2)
#define INCOMING_ENTITY (-1)

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
#define CROCCODILE_MOVE_X 1
#define CROCCODILE_ID_INDEX 0
#define CROCCODILE_STREAM_INDEX 1
#define CROCCODILE_SPEED_INDEX 2
#define MIN_CROCCODILE_SPAWN_TIME 1
#define MAX_CROCCODILE_SPAWN_TIME 4
#define BAD_THRESHOLD DIFF_SET+2
#define BUBBLE_THRESHOLD 1000
#define CROCCODILE_GOOD_SIG 0
#define CROCCODILE_BAD_SIG 1
#define CROCCODILE_BUBBLE_SIG 2
#define CROCCODILE_IMMERSION_SIG 3

// Plant constants
#define PLANT_ID_INDEX 0
#define PLANT_X_INDEX 1
#define PLANT_SPAWN_SIG 0
#define PLANT_OPEN_SIG 1
#define PLANT_SHOT_SIG 2
#define PLANT_CLOSE_SIG 3
#define PLANT_SHOT_INTERVAL (N_DIFFICULTY - DIFF_SET)

// Bullet constants
#define BULLET_ID_INDEX 0
#define BULLET_Y_INDEX 1
#define BULLET_X_INDEX 2
#define BULLET_MOVE_Y 1
#define BULLET_SPEED 10

// Function prototypes
Game_t play_manche(int score, int n_lifes, bool* holes_occupied);