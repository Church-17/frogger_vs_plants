// Include libs
#include "main.h"
#include "game.h"

// Time constants
#define TIME_COL 0
#define TIME_YELLOW 30
#define TIME_RED 10
#define TIMEBAR_LEN 15
#define STRLEN_TIME 3
#define TIMEBAR_COL (TIME_COL+4)

// Lifes constants
#define SPACE_PER_LIFE 3
#define LIFES_COL (MAIN_COLS - SPACE_PER_LIFE * MAX_N_LIFES)

// Hole
#define HOLE_DIM_X (FROG_DIM_X + 2)
#define HOLE_DIM_Y FROG_DIM_Y
#define HOLE_PAD_X ((MAIN_COLS/N_HOLES - HOLE_DIM_X)/2)

// Frog constant
#define FROG_DIM_X 10
#define FROG_DIM_Y 4
#define MOVE_FROG_Y FROG_DIM_Y
#define MOVE_FROG_X 2

// Function prototypes
void print_lifes(int n_lifes);
void print_time(int time_remained);
void print_frog(const Game_t* gamaver);
bool in_hole_line(Position frog);