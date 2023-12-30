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
void async_exec(List_thread* tids, int index, void (*func_process)(int*), int* func_params) {
    
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

void quit_manche(void) {

}
