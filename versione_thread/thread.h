// Include libs
#include <time.h>
#include "../struct_proto.h"

// Define constants

// Function prototypes
void async_exec(List_thread* tids, int index, void* (*func_process)(void*), void* func_params);
void read_msg(Message* buf);
void write_msg(Message* buf);
void msleep(time_t timer);
void pause_manche(void);
void resume_manche(void);
void quit_manche(const List_thread tids);
