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
void piper(int* pipe_fds);
void forker(List_pid* pids, void (*func_process)(WINDOW*, int, int, int*), WINDOW* win, int* pipe_fds, int index, int* other_params);
void reader(int pipe_read, Message* buf);
void writer(int pipe_write, Message* buf);

// --- PROCESS TYPES ---
#define RESIZE_ID -3
#define CLOSE_ID -2
#define PAUSE_ID -1
#define FROG_ID 0
#define TIME_ID 1