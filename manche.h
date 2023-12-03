// Include libs
#include "struct_proto.h"

#define MANCHE_LOST 0
#define MANCHE_RETR -1
#define MANCHE_CLOSE -2
#define MANCHE_QUIT -3

int play_manche(bool* holes_occupied, int n_lifes);