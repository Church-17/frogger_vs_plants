#include "../struct_proto.h"

#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_DIM 2

void signal_all(const List_pid *pids, int signal);
void quit_all(int err_code, const List_pid *pids);
void forker(List_pid* pids);
void piper(int* pipe_fds);