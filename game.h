// Include libs
#include "struct_proto.h"

// Define constant
#define N_LIFES 3
#define N_HOLES 5
#define CLOSE_GAME_KEY 'q'
#define PAUSE_GAME_KEY 'p'
#define LINE_BANK_1 0
#define LINE_RIVER 4
#define LINE_BANK_2 (MAIN_ROWS-4)

// Function prototypes
void print_bg_frog(void);
bool game(void);