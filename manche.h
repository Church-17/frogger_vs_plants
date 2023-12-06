// Include libs
#include "game.h"
#include "main.h"
#include "sprites.h"
#include "struct_proto.h"

// Manche codes
#define MANCHE_LOST 0
#define MANCHE_RETR -1
#define MANCHE_CLOSE -2
#define MANCHE_QUIT -3

// Frog constant
#define INIT_FROG_Y LINE_BANK_2
#define INIT_FROG_X (MAIN_COLS/2-FROG_X_DIM/2)
#define MOVE_FROG_Y 4
#define MOVE_FROG_X 2
#define LIM_UP LINE_BANK_1
#define LIM_DOWN INIT_FROG_Y
#define LIM_LEFT 0
#define LIM_RIGHT (MAIN_COLS-FROG_X_DIM)

// Time constants
#define HEADER_ROW 0
#define TIME_COL 0
#define TIME_MANCHE 10
#define TIME_YELLOW 30
#define TIME_RED 10
#define TIMEBAR_LEN 15
#define STRLEN_TIME 3
#define TIMEBAR_COL (TIME_COL+4)

// Lifes constants
#define LIFE_DIM 1
#define SPACE_PER_LIFE 3
#define LIFES_SPACE (SPACE_PER_LIFE * MAX_N_LIFES)
#define LIFES_COL (MAIN_COLS - LIFES_SPACE)

int play_manche(bool* holes_occupied, int* n_lifes);