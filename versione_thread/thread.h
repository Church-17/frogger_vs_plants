// Include libs
#include <time.h>
#include <semaphore.h>
#include "../struct_proto.h"

// Define constants

// Function prototypes
void init_semaphore(void);
void destroy_semaphore(void);
void async_exec(List_thread* tids, int index, void* (*func_thread)(void*), int* func_params);
Message read_msg(int* rd_index);
void write_msg(Message msg);
void msleep(time_t timer);
void check_pause(void);
void pause_manche(void);
void resume_manche(void);
void quit_manche(const List_thread tids);
