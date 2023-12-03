// Include libs
#include "../utils.h"
#include "../struct.h"
#include "process.h"

// Sends a signal to all the processes
void signal_all(const List_pid pids, int signal) {
    for(int i = 0; i < pids.len; i++) {
        kill(pids.list[i], signal);
    }
}

// Ends the game
void quit_all(int err_code, const List_pid pids) {
    signal_all(pids, SIGKILL);
    quit(err_code);
}

// Calls fork() handling the errors
pid_t forker(List_pid* pids) {
    pid_t pid = fork();
    if(pid < 0) {
        quit_all(ERR_FORK, *pids);
    }
    return pid;
}

// Calls pipe() handling the errors
void piper(int* pipe_fds) {
    if(pipe(pipe_fds) < 0) {
        quit(ERR_PIPE);
    }
}