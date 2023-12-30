// Include libs
#include "../struct_proto.h"

// Define constants

// Function prototypes
void signal_all(const List_thread tids, int signal);
void async_exec(List_thread* tids, int index, void (*func_process)(int*), int* func_params);
void read_msg(Message* buf);
void write_msg(Message* buf);
