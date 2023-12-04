#include "../struct_proto.h"

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

#define MOVE_FROG_Y 4
#define MOVE_FROG_X 2
#define LIM_UP 0
#define LIM_DOWN (MAIN_ROWS-FROG_Y_DIM)
#define LIM_LEFT 0
#define LIM_RIGHT (MAIN_COLS-FROG_X_DIM)

void frog_process(int pipe_write);