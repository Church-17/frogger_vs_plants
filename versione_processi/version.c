// Include libs
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "../utils.h"
#include "../struct.h"
#include "../version.h"

// Define constant
#define PID_CHILD 0
#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_DIM 2
#define MSLEEP_INTEVAL 10

// Define process variables
int pipe_fds[PIPE_DIM];
List_pid entities_pids;

// Funtion prototypes
void signal_all(int signal);
void msleep(time_t timer);

// Calls pipe() handling the errors & initilize pids array
void init_version(int n_alloc) {
    if(pipe(pipe_fds) < 0) {
        quit(ERR_PIPE);
    }
    alloc(pid_t, entities_pids.list, n_alloc);
    entities_pids.len = n_alloc;
    for(int i = 0; i < n_alloc; i++) {
        entities_pids.list[i] = 0;
    }
}

// Calls fork() handling the errors
void async_exec(int index, void (*func_process)(int*), int* func_params) {
    pid_t pid = fork();
    if(pid < 0) {
        quit_manche();
        quit(ERR_FORK);
    }
    if(pid == PID_CHILD) {
        close(pipe_fds[PIPE_READ]);
        func_process(func_params);
        _exit(NO_ERR);
    }
    entities_pids.list[index] = pid;
}

void read_msg(Message* msg) {
    while(read(pipe_fds[PIPE_READ], msg, sizeof(Message)) < 0) {
        if(errno != EINTR) quit(ERR_READ);
    }
}

void write_msg(const Message* msg) {
    while(write(pipe_fds[PIPE_WRITE], msg, sizeof(Message)) < 0) {
        if(errno != EINTR) quit(ERR_WRITE);
    }
}

void kill_entity(int index) {
    waitpid(entities_pids.list[index], NULL, 0);
    entities_pids.list[index] = 0;
}

void signal_entity(int index, int signal) {
    kill(entities_pids.list[index], signal);
}

// Sends a signal to all the entities
void signal_all(int signal) {
    for(int i = 0; i < entities_pids.len; i++) {
        if(entities_pids.list[i] != 0) {  
            kill(entities_pids.list[i], signal);
        }
    }
}

void pause_manche(void) {
    signal_all(SIGSTOP);
}

void resume_manche(void) {
    signal_all(SIGCONT);
}

// Ends the game
void quit_manche(void) {
    signal_all(SIGKILL);
    free(entities_pids.list);
    entities_pids.list = NULL;
    close(pipe_fds[PIPE_READ]);
    close(pipe_fds[PIPE_WRITE]);
    while(wait(NULL) > 0); // Wait all child processes
}

// Sleep for certain amount of milliseconds, handling interrupts
void msleep(time_t timer) {
    for(int dec = 0; dec < MSLEEP_INTEVAL; dec++) {
        usleep(timer * MSEC_IN_SEC / MSLEEP_INTEVAL);
    }
}
