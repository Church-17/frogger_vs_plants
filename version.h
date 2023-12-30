// Include libs
#include <time.h>
#include "struct_proto.h"

// Function prototypes
void init_version(int n_alloc);
void async_exec(int index, void (*func_process)(int*), int* func_params);
void read_msg(Message* msg);
void write_msg(const Message* msg);
void kill_entity(int index);
void signal_entity(int index, int signal);
void pause_manche(void);
void resume_manche(void);
void quit_manche(void);
void msleep(time_t timer);
