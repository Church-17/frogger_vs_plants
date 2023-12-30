// Include libs
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "../utils.h"
#include "../struct.h"
#include "process.h"

// Function prototypes
void quit_all(int err_code, const List_pid pids);

// Sends a signal to all the processes
void signal_all(const List_pid pids, int signal) {
    for(int i = 0; i < pids.len; i++) {
        if(pids.list[i] != 0) {  
            kill(pids.list[i], signal);
        }
    }
}

// Ends the game
void quit_all(int err_code, const List_pid pids) {
    signal_all(pids, SIGKILL);
    quit(err_code);
}

// Calls pipe() handling the errors
void piper(int* pipe_fds) {
    if(pipe(pipe_fds) < 0) {
        quit(ERR_PIPE);
    }
}

// Calls fork() handling the errors
void forker(int* pipe_fds, List_pid* pids, int index, void (*func_process)(int, int*), int* func_params) {
    pid_t pid = fork();
    if(pid < 0) {
        quit_all(ERR_FORK, *pids);
    }
    if(pid == PID_CHILD) {
        close(pipe_fds[PIPE_READ]);
        func_process(pipe_fds[PIPE_WRITE], func_params);
        _exit(NO_ERR);
    }
    pids->list[index] = pid;
}

void reader(int pipe_read, Message* buf) {
    while(read(pipe_read, buf, sizeof(Message)) < 0) {\
        if(errno != EINTR) quit(ERR_READ);\
    }
}

void writer(int pipe_write, Message* buf) {
    while(write(pipe_write, buf, sizeof(Message)) < 0) {\
        if(errno != EINTR) quit(ERR_WRITE);\
    }
}
