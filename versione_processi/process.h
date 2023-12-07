// Include libs
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "../utils.h"

// Define constant
#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_DIM 2

// Define macros
#define reader(pipe_read, buf, pids) if(read(pipe_read, &buf, sizeof(buf)) < 0) quit_all(ERR_READ, pids)
#define writer(pipe_write, buf, pids) if(write(pipe_write, &buf, sizeof(buf)) < 0) quit_all(ERR_WRITE, pids)

// Function prototypes
void signal_all(const List_pid pids, int signal);
void quit_all(int err_code, const List_pid pids);
void forker(int index, List_pid* pids, void (*func_process)(int), int* pipe_fds);
void piper(int* pipe_fds);

// --- PROCESS TYPES ---
#define RESIZE_ID -3
#define CLOSE_ID -2
#define PAUSE_ID -1
#define FROG_ID 0
#define TIME_ID 1