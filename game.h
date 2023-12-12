// Include libs
#include "struct_proto.h"

// Define constant
#define PAUSE_GAME_KEY 'p'
#define CLOSE_GAME_KEY 'q'
#define TIME_MANCHE 60
#define N_LIFES 3
#define N_HOLES 5
#define N_WATER_STREAM 8
#define N_PLANTS 3

// Define colors
#define HEDGE_BG (GREY_BLACK | A_STANDOUT)
#define BANK_BG GREEN_PURPLE
#define RIVER_BG DARKGREEN_BLUE
#define GOOD_CROCCODILE_BG GREEN_DARKGREEN
#define BAD_CROCCODILE_BG GREEN_BOURDEAUX

// Function prototypes
void demo(void);
bool game(void);
void print_game(const Game_t* gamevar);