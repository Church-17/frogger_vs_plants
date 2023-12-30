// Include libs
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "../utils.h"
#include "../struct.h"
#include "process.h"

// Function prototypes
void quit_all(int err_code, const List_thread tids);

// Sends a signal to all the processes
void signal_all(const List_thread tids, int signal) {
    
}

// Ends the game
void quit_all(int err_code, const List_thread tids) {
    
    quit(err_code);
}

// Calls fork() handling the errors
void async_exec(List_thread* tids, int index, void (*func_process)(int*), int* func_params) {
    
}

void read_msg(Message* buf) {
    
}

void write_msg(Message* buf) {
    
}
