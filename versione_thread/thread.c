// Include libs
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include "../utils.h"
#include "../struct.h"
#include "thread.h"

// Define constants
#define MSLEEP_INTEVAL 10
#define DIM_BUFFER 16

// Global variables
Message buffer[DIM_BUFFER];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_free, sem_occupied;

// Calls fork() handling the errors
void async_exec(List_thread* tids, int index, void* (*func_thread)(void*), int* func_params) {
    if(pthread_create(&(tids->list[index]), NULL, &func_thread, (void*) func_params) != 0) {
        tids->list[index] = 0;
        quit_manche(*tids);
        quit(ERR_INIT_THREAD);
    }
}

void read_msg(Message* msg) {
    static int rd_index = 0;
    while(sem_wait(&sem_occupied) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_WAIT);
    }
    if(pthread_mutex_lock(&mutex) != 0) { // Handle mutex lock error
        quit(ERR_MUTEX_LOCK);
    }
    *msg = buffer[rd_index];
    rd_index = mod(rd_index + 1, DIM_BUFFER);
    if(pthread_mutex_unlock(&mutex) != 0) { // Handle mutex unlock error
        quit(ERR_MUTEX_UNLOCK);
    }
    while(sem_post(&sem_free) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_POST);
    }
}

void write_msg(Message* msg) {
    static int wr_index = 0;
    while(sem_wait(&sem_free) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_WAIT);
    }
    if(pthread_mutex_lock(&mutex) != 0) { // Handle mutex lock error
        quit(ERR_MUTEX_LOCK);
    }
    buffer[wr_index] = *msg; // Write message in buffer
    wr_index = mod(wr_index + 1, DIM_BUFFER); // Increment wr index
    if(pthread_mutex_unlock(&mutex) != 0) { // Handle mutex unlock error
        quit(ERR_MUTEX_UNLOCK);
    }
    while(sem_post(&sem_occupied) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_POST);
    }
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
