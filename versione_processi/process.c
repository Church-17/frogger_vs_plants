#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "../utils.h"
#include "../struct.h"

void signal_all(const List_pid *pids, int signal) {
    for(int i = 0; i < pids->len; i++) {
        kill(pids->list[i], signal);
    }
}

void quit_all(int err_code, const List_pid *pids) {
    signal_all(pids, SIGKILL);
    quit(err_code);
}

void forker(List_pid* pids) {
    pid_t pid = fork();
    if(pid < 0) {
        quit_all(ERR_FORK, pids);
    }
    // Adding pid to list
    pids->list[pids->len] = pid;
    (pids->len)++;
}

void piper(int* pipe_fds) {
    int ret = pipe(pipe_fds);
    if(ret < 0) {
        quit(ERR_PIPE);
    }
}