// Include libs
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
void forker(int* pipe_fds, List_pid* pids, int index, void (*func_process)(int, int*), int* func_params);
void reader(int pipe_read, Message* buf);
void writer(int pipe_write, Message* buf);
