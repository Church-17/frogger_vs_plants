// Include libs
#include "struct_proto.h"

// Define constant
#define PAUSE_GAME_KEY 'p'
#define CLOSE_GAME_KEY 'q'
#define TIME_MANCHE 60
#define N_LIFES 3
#define MAX_N_LIFES 6
#define N_HOLES 5
#define N_WATER_STREAM 4

// Define game zones
#define LINE_HEADER 0
#define LINE_HEDGE 1
#define LINE_HOLES 2
#define LINE_BANK_1 6
#define LINE_RIVER 10
#define LINE_BANK_2 26

// Function prototypes
void print_background(void);
bool game(void);
void print_game(const Game_t* gamevar);