// Include libs
#include "sprites.h"

// Manche codes
#define MANCHE_LOST 0
#define MANCHE_RETR -1
#define MANCHE_CLOSE -2
#define MANCHE_QUIT -3

// Frog constant
#define INIT_FROG_Y LINE_BANK_2
#define INIT_FROG_X (MAIN_COLS/2-FROG_DIM_X/2)
#define LIM_UP LINE_HOLES
#define LIM_DOWN LINE_BANK_2
#define LIM_LEFT 0
#define LIM_RIGHT (MAIN_COLS-FROG_DIM_X)
#define FROG_NOT_ON_CROCCODILE (-1)

// Croccodile constant
#define FREE_CROCCODILE (-1)
#define INCOMING_CROCCODILE (-2)

Game_t play_manche(bool* holes_occupied, int n_lifes);