// Include libs
#include "../res.h"

// Define constants
#define MOVE_CROCCODILE_X 2
#define CROCCODILE_ID_INDEX 0
#define CROCCODILE_STREAM_INDEX 1
#define CROCCODILE_SPEED_INDEX 2
#define N_CROCCODILE_PARAMS 3
#define MIN_CROCCODILE_SPAWN_TIME 1
#define MAX_CROCCODILE_SPAWN_TIME 4
#define BAD_THRESHOLD DIFF_SET+2
#define BUBBLE_THRESHOLD 1000

// Function prototypes
void croccodile_process(int pipe_write, int* other_params);