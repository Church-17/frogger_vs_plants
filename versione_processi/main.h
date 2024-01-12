// Include libs
#include <ncurses.h>
#include "struct_proto.h"

// Inter-object vars
extern WINDOW* main_scr;

// Function prototypes
bool resize_proc(WINDOW* win, int dim_y, int dim_x, const Game_t* gamevar);