// Include libs
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include "utils.h"
#include "struct.h"
#include "thread.h"

// Define constants
#define MSLEEP_INTEVAL 10
#define DIM_BUFFER 16

// Global variables
bool game_in_pause = FALSE;
int wr_index = 0;
Message buffer[DIM_BUFFER];
pthread_mutex_t wr_buf_mutex = PTHREAD_MUTEX_INITIALIZER, pause_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pause_cond = PTHREAD_COND_INITIALIZER;
sem_t sem_free, sem_occupied;

// Lock mutex
void mutex_lock(pthread_mutex_t* mutex) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if(pthread_mutex_lock(mutex) != 0) { // Handle mutex lock error
        quit(ERR_MUTEX_LOCK);
    }
}

// Unlock mutex
void mutex_unlock(pthread_mutex_t* mutex) {
    if(pthread_mutex_unlock(mutex) != 0) { // Handle mutex lock error
        quit(ERR_MUTEX_UNLOCK);
    }
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

// Create thread handling errors
void async_exec(List_thread* tids, int index, void* (*func_thread)(void*), int* func_params) {
    if(pthread_create(&(tids->list[index]), NULL, func_thread, (void*) func_params) != 0) {
        tids->list[index] = 0;
        quit_manche(*tids);
        quit(ERR_INIT_THREAD);
    }
}

// Init semaphores handling errors & reset wr_index needed
void init_semaphore(void) {
    wr_index = 0;
    if(sem_init(&sem_free, 0, DIM_BUFFER) != 0) {
        quit(ERR_SEM_INIT);
    }
    if(sem_init(&sem_occupied, 0, 0) != 0) {
        quit(ERR_SEM_INIT);
    };
}

// Free semaphores handling errors
void destroy_semaphore(void) {
    if(sem_destroy(&sem_free) != 0) {
        quit(ERR_SEM_DESTROY);
    };
    if(sem_destroy(&sem_occupied) != 0) {
        quit(ERR_SEM_DESTROY);
    };
}

// Read message from buffer (only 1 consumer intended)
Message read_msg(int* rd_index) {
    Message msg;
    while(sem_wait(&sem_occupied) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_WAIT);
    }
    msg = buffer[*rd_index];
    *rd_index = mod(*rd_index + 1, DIM_BUFFER);
    while(sem_post(&sem_free) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_POST);
    }
    return msg;
}

// Write message in buffer
void write_msg(Message msg) {
    while(sem_wait(&sem_free) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_WAIT);
    }
    mutex_lock(&wr_buf_mutex);
    buffer[wr_index] = msg; // Write message in buffer
    wr_index = mod(wr_index + 1, DIM_BUFFER); // Increment wr index
    mutex_unlock(&wr_buf_mutex);
    while(sem_post(&sem_occupied) != 0) { // Handle sem wait error, passing signal interrupt
        if(errno != EINTR) quit(ERR_SEM_POST);
    }
}

// Sleep for certain amount of milliseconds, handling interrupts & checking pause
void msleep(time_t timer) {
    for(int dec = 0; dec < MSLEEP_INTEVAL; dec++) {
        check_pause();
        usleep(timer * MSEC_IN_SEC / MSLEEP_INTEVAL);
    }
}

// Check if game is in pause
void check_pause(void) {
    if(game_in_pause) {
        mutex_lock(&pause_mutex);
        if(pthread_cond_wait(&pause_cond, &pause_mutex) != 0) { // Wait until game resume
            quit(ERR_MUTEX_COND);
        }
        mutex_unlock(&pause_mutex);
    }
}

// Pause game
void pause_manche(void) {
    game_in_pause = TRUE;
}

// Resume game
void resume_manche(void) {
    game_in_pause = FALSE;
    pthread_cond_broadcast(&pause_cond);
}

// Quit game canceling all threads
void quit_manche(const List_thread tids) {
    for(int i = 0; i < tids.len; i++) {
        if(tids.list[i] != 0) {
            pthread_cancel(tids.list[i]);
            pthread_join(tids.list[i], NULL);
        }
    }
}
