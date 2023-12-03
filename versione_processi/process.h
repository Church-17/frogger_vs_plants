// Include libs
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "../struct_proto.h"

// Define constant
#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_DIM 2

// Function prototypes
void signal_all(const List_pid pids, int signal);
void quit_all(int err_code, const List_pid pids);
pid_t forker(List_pid* pids);
void piper(int* pipe_fds);

// --- PROCESS TYPES ---
#define FROG_ID 0
#define PAUSE_ID -1
#define CLOSE_ID -2