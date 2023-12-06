// Include libs
#include "struct_proto.h"

// Define constant
#define N_LIFES 3
#define MAX_N_LIFES 6
#define N_HOLES 5
#define CLOSE_GAME_KEY 'q'
#define PAUSE_GAME_KEY 'p'
#define LINE_BANK_1 1
#define LINE_RIVER 9
#define LINE_BANK_2 (MAIN_ROWS-5)

// Function prototypes
void print_background(void);
bool game(void);