// Include libs
#include <time.h>
#include "../struct_proto.h"

// Define constant
#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_DIM 2

// Function prototypes
void async_exec(int* pipe_fds, List_pid* pids, int index, void (*func_process)(int, int*), int* func_params);
Message read_msg(int pipe_read);
void write_msg(int pipe_write, Message msg);
void msleep(time_t timer);
void signal_all(const List_pid pids, int signal);
