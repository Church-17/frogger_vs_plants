// Include libs
#include <time.h>
#include "../struct_proto.h"

// Define constants

// Function prototypes
void async_exec(List_thread* tids, int index, void* (*func_thread)(void*), int* func_params);
void read_msg(Message* msg);
void write_msg(Message* msg);
void msleep(time_t timer);
void pause_manche(void);
void resume_manche(void);
void quit_manche(const List_thread tids);
