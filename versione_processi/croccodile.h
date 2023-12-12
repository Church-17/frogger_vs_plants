#include "../struct_proto.h"

#define MOVE_CROCCODILE_X 2
#define CROCCODILE_ID_INDEX 0
#define CROCCODILE_STREAM_INDEX 1
#define CROCCODILE_SPEED_INDEX 2
#define N_CROCCODILE_PARAMS 3
#define MIN_CROCCODILE_SPAWN_TIME 1
#define MAX_CROCCODILE_SPAWN_TIME 4

void croccodile_process(int pipe_write, int* other_params);