// Include libs
#include "struct_proto.h"

// Define constant
#define MAIN_ROWS 30
#define MAIN_COLS 100

// Inter-object vars
extern WINDOW* main_scr;

// Function prototypes
bool resize_proc(WINDOW* win, int dim_y, int dim_x, const Game_t* gamevar);