// Include libs
#include "struct_proto.h"
#include "main.h"
#include "sprites.h"

// Define constant
#define N_LIFES 3
#define N_HOLES 5
#define CLOSE_GAME_KEY 'q'
#define PAUSE_GAME_KEY 'p'
#define LINE_BANK_1 0
#define LINE_RIVER 4
#define LINE_BANK_2 (MAIN_ROWS-4)

#define INIT_FROG_Y LINE_BANK_2
#define INIT_FROG_X (MAIN_COLS/2-FROG_X_DIM/2)

// Function prototypes
void print_bg_frog(void);
bool game(void);