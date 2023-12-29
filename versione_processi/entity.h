// Define constants
#define FROG_ON_CROCCODILE_SIG SIGUSR1

// Function prototypes
void time_process(int pipe_write, int* other_params);
void frog_process(int pipe_write, int* other_params);
void croccodile_process(int pipe_write, int* other_params);
void plant_process(int pipe_write, int* other_params);
void bullet_process(int pipe_write, int* other_params);