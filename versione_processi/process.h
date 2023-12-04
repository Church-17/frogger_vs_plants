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
#define SIG_CLOSE -2
#define SIG_PAUSE -1
#define SIG_FROG 0
#define SIG_TIME 1