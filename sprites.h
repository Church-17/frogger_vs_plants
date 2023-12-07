#include "struct_proto.h"

// Frog
#define FROG_X_DIM 10
#define FROG_Y_DIM 4

void print_lifes(const int n_lifes);
void print_time(const int time_remained, const bool reprint_bar);
void print_frog(const Game_t* gamaver);
void print_holes(int num_hole, bool* occupied);
bool in_hole(Position frog);
bool in_hole_line(Position frog);