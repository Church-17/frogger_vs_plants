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
#define INIT_FROG_Y LINE_BANK_2
#define INIT_FROG_X (MAIN_COLS/2-FROG_DIM_X/2)
#define LIM_UP LINE_HOLES
#define LIM_DOWN LINE_BANK_2
#define LIM_LEFT 0
#define LIM_RIGHT (MAIN_COLS-FROG_DIM_X)
#define FROG_NOT_ON_CROCCODILE (-1)
#define FROG_POSITION_SIG 0
#define FROG_SHOT_SIG 1

// Croccodile constants
#define FREE_CROCCODILE (-1)
#define INCOMING_CROCCODILE (-2)
#define GOOD_CROCCODILE_SIG 0
#define BAD_CROCCODILE_SIG 1
#define BUBBLE_CROCCODILE_SIG 2
#define IMMERSION_CROCCODILE_SIG 3

// Plant constants
#define INCOMING_PLANT (-1)

// Bullet constants
#define FREE_BULLET (-1)
#define INCOMING_BULLET (-2)

Game_t play_manche(int score, int n_lifes, bool* holes_occupied);