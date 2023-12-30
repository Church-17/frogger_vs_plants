// Include libs
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "../utils.h"
#include "../struct.h"
#include "thread.h"

// Define constants
#define MSLEEP_INTEVAL 10

// Calls fork() handling the errors
void async_exec(List_thread* tids, int index, void* (*func_process)(void*), void* func_params) {
    pthread_create(&(tids->list[index]), NULL, &func_params, func_params);
}

void read_msg(Message* buf) {
    
}

void write_msg(Message* buf) {
    
}

// Sleep for certain amount of milliseconds, handling interrupts
void msleep(time_t timer) {
    for(int dec = 0; dec < MSLEEP_INTEVAL; dec++) {
        usleep(timer * MSEC_IN_SEC / MSLEEP_INTEVAL);
    }
}

void pause_manche(void) {

}

void resume_manche(void) {

}

void quit_manche(const List_thread tids) {
    for(int i = 0; i < tids.len; i++) {
        if(tids.list[i] != 0) {
            pthread_cancel(tids.list[i]);
        }
    }
}
