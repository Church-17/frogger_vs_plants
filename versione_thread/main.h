// Include libs
#include "struct.h"

// Define constant
#define MAIN_ROWS 15
#define MAIN_COLS 50

// Inter-object vars
extern bool in_game_status;
extern WINDOW* main_scr;

// Function prototypes
bool resize_proc(WINDOW* win, int dim_y, int dim_x);