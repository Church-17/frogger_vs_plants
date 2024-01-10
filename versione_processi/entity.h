// Include libs
#include <signal.h>

// Define constants
#define FROG_ON_CROCCODILE_SIG SIGUSR1
#define CROCCODILE_SHOTTED_SIG SIGUSR2

// Function prototypes
void time_process(int pipe_write, int* params);
void frog_process(int pipe_write, int* params);
void croccodile_process(int pipe_write, int* params);
void plant_process(int pipe_write, int* params);
void bullet_process(int pipe_write, int* params);