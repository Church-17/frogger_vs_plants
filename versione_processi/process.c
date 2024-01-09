// Include libs
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "../utils.h"
#include "../struct.h"
#include "process.h"

// Define constants
#define MSLEEP_INTEVAL 10

// Calls fork() handling the errors
void async_exec(int* pipe_fds, List_pid* pids, int index, void (*func_process)(int, int*), int* func_params) {
    pid_t pid = fork();
    if(pid < 0) {
        signal_all(*pids, SIGKILL);
        quit(ERR_FORK);
    }
    if(pid == PID_CHILD) {
        close(pipe_fds[PIPE_READ]);
        func_process(pipe_fds[PIPE_WRITE], func_params);
        _exit(NO_ERR);
    }
    pids->list[index] = pid;
}

void read_msg(int pipe_read, Message* msg) {
    while(read(pipe_read, msg, sizeof(Message)) != 0) {
        if(errno != EINTR) quit(ERR_READ);
    }
}

void write_msg(int pipe_write, Message* msg) {
    while(write(pipe_write, msg, sizeof(Message)) != 0) {
        if(errno != EINTR) quit(ERR_WRITE);
    }
}

// Sleep for certain amount of milliseconds, handling interrupts
void msleep(time_t timer) {
    for(int dec = 0; dec < MSLEEP_INTEVAL; dec++) {
        usleep(timer * MSEC_IN_SEC / MSLEEP_INTEVAL);
    }
}

// Sends a signal to all the processes
void signal_all(const List_pid pids, int signal) {
    for(int i = 0; i < pids.len; i++) {
        if(pids.list[i] != 0) {  
            kill(pids.list[i], signal);
        }
    }
}
