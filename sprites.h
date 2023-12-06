#include "struct_proto.h"

// Frog

#define FROG_X_DIM 10
#define FROG_Y_DIM 4

void print_lifes(int n_lifes);
void print_time(int time_remained, int new_timebar_len);
void print_frog(WINDOW* win, int y, int x, int* colors_bg);