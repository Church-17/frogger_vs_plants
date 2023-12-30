// Include libs
#include <unistd.h>
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "entity.h"

// Define constants
#define MSLEEP_INTEVAL 10

// Global vars
bool frog_on_me = FALSE;

// Function prototypes
void frog_stepped_on_me(int sig);
void msleep(time_t timer);

void time_process(int pipe_write, int* other_params) {
    // Init vars
    time_t start = timestamp(), end = start;
    Message msg;
    msg.id = TIME_ID;
    msg.sig = TIME_MANCHE;

    // Loop for beat time
    while(TRUE) {
        if(end - start >= MSEC_IN_SEC) { // If a seconds passed...
            start = end; // Update start
            msg.sig--;
            writer(pipe_write, &msg); // Write in pipe
        }
        msleep(MSEC_IN_SEC); // Sleep for a second
        end = timestamp(); // Update end
    }
}

void frog_process(int pipe_write, int* other_params) {
    // Init vars
    bool do_send_msg = FALSE;
    int key;
    Message msg;
    msg.id = FROG_ID;
    msg.sig = FROG_POSITION_SIG;

    // Init window from which to take input
    WINDOW* input_win = new_win(1, 1, 0, 0);

    // Frog loop to get pressed key
    while(TRUE) {
        key = wgetch(input_win);
        switch(key) {
            case MOVE_N_KEY:
            case MOVE_N_KEY - CAPITAL_SHIFT:
            case KEY_UP:
                msg.y = -FROG_MOVE_Y;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send msg
                break;
                
            case MOVE_S_KEY:
            case MOVE_S_KEY - CAPITAL_SHIFT:
            case KEY_DOWN:
                msg.y = FROG_MOVE_Y;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case MOVE_W_KEY:
            case MOVE_W_KEY - CAPITAL_SHIFT:
            case KEY_LEFT:
                msg.y = 0;
                msg.x = -FROG_MOVE_X;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case MOVE_E_KEY:
            case MOVE_E_KEY - CAPITAL_SHIFT:
            case KEY_RIGHT:
                msg.y = 0;
                msg.x = FROG_MOVE_X;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case PAUSE_GAME_KEY:
            case PAUSE_GAME_KEY - CAPITAL_SHIFT:
                msg.id = PAUSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case CLOSE_GAME_KEY:
            case CLOSE_GAME_KEY - CAPITAL_SHIFT:
                msg.id = CLOSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case SHOT_GAME_KEY:
                msg.sig = FROG_SHOT_SIG;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case KEY_RESIZE:
                msg.id = RESIZE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            default: break; 
        }
        if(do_send_msg) {
            writer(pipe_write, &msg);
            msg.id = FROG_ID;
            msg.sig = FROG_POSITION_SIG;
            do_send_msg = FALSE;
        }
    }
}

// params: [communication_id, n_stream, speed_stream]
void croccodile_process(int pipe_write, int* other_params) {
    // Init vars
    bool do_exit = FALSE, do_immersion = FALSE;
    int n_stream, speed_stream, immersion_time;
    time_t start, end;
    Message msg;

    // Handle signal of frog steps on this croccodile
    signal(FROG_ON_CROCCODILE_SIG, &frog_stepped_on_me);

    // Unpack croccodile params
    msg.id = other_params[CROCCODILE_ID_INDEX];
    n_stream = other_params[CROCCODILE_STREAM_INDEX];
    speed_stream = other_params[CROCCODILE_SPEED_INDEX];

    // Determine coordinates
    msg.y = LINE_RIVER + FROG_DIM_Y * n_stream;
    msg.x = speed_stream > 0 ? -CROCCODILE_DIM_X + CROCCODILE_MOVE_X : MAIN_COLS - CROCCODILE_MOVE_X;

    // Random croccodile kind & spawn time
    srand(timestamp() + msg.id);
    msg.sig = rand_range(0, 10) < BAD_THRESHOLD ? CROCCODILE_BAD_SIG : CROCCODILE_GOOD_SIG;
    msleep(rand_range(MIN_CROCCODILE_SPAWN_TIME, MAX_CROCCODILE_SPAWN_TIME) * MSEC_IN_SEC);

    // Write initial position
    writer(pipe_write, &msg);

    // Loop for write new coordinates
    while(!do_exit) {
        // Update X coordinate
        if(speed_stream > 0) {
            msg.x += CROCCODILE_MOVE_X;
            if(msg.x >= MAIN_COLS) {
                do_exit = TRUE;
            }
        } else {
            msg.x -= CROCCODILE_MOVE_X;
            if(msg.x <= -CROCCODILE_DIM_X) {
                do_exit = TRUE;
            }
        }

        if(frog_on_me && msg.sig >= CROCCODILE_BAD_SIG) { // If croccodile is bad and frog stepped on...
            if(!do_immersion) { // If immersion not started, start it
                immersion_time = rand_range(2, 4) * MSEC_IN_SEC;
                start = timestamp();
                do_immersion = TRUE;
            }
            end = timestamp();
            if(end - start >= immersion_time) { // If the croccodile have to immerge...
                msg.sig = CROCCODILE_IMMERSION_SIG;
                do_exit = TRUE;
            } else if(end - start >= immersion_time - BUBBLE_THRESHOLD) { // If there is BUBBLE_THRESHOLD left to immersion...
                msg.sig = CROCCODILE_BUBBLE_SIG;
            }
        }

        msleep(MSEC_IN_SEC * CROCCODILE_MOVE_X / (speed_stream > 0 ? speed_stream : -speed_stream)); // Sleep based on speed
        writer(pipe_write, &msg); // Write on pipe
    }
    return;
}

void plant_process(int pipe_write, int* other_params) {
    // Init vars
    time_t bullet_interval;
    Message msg;

    // Unpack croccodile params
    msg.id = other_params[PLANT_ID_INDEX];
    msg.y = LINE_BANK_1;
    msg.x = other_params[PLANT_X_INDEX];
    msg.sig = PLANT_SPAWN_SIG;

    // Random spawn time & shot interval
    srand(timestamp() + msg.id);
    msleep(rand_range(1, 5) * MSEC_IN_SEC);
    bullet_interval = rand_range(1, 5) * MSEC_IN_SEC;

    writer(pipe_write, &msg);

    msg.sig = PLANT_BULLET_SIG;

    // Plant loop to shot bullets
    while(TRUE) {
        msleep(bullet_interval);
        writer(pipe_write, &msg);
    }
}

// params: [communication_id, y, x]
void bullet_process(int pipe_write, int* other_params) {
    // Init vars
    bool do_exit = FALSE;
    Message msg;

    // Unpack bullet params
    msg.id = other_params[BULLET_ID_INDEX];
    msg.y = other_params[BULLET_Y_INDEX];
    msg.x = other_params[BULLET_X_INDEX];

    // Write initial position
    writer(pipe_write, &msg);
    if(msg.y < LINE_BANK_1 || msg.y >= MAIN_ROWS) {
        do_exit = TRUE;
    }

    while(!do_exit) {
        // Update Y coordinate
        if(msg.id < MIN_PLANT_BULLET_ID) { // If it is a frog bullet...
            msg.y -= BULLET_MOVE_Y;
            if(msg.y < LINE_BANK_1) {
                do_exit = TRUE;
            }
        } else { // If it is a plant bullet...
            msg.y += BULLET_MOVE_Y;
            if(msg.y >= MAIN_ROWS) {
                do_exit = TRUE;
            }
        }

        msleep(MSEC_IN_SEC / BULLET_SPEED);
        writer(pipe_write, &msg);
    }

}

void frog_stepped_on_me(int sig) { // If frog steps on this croccodile
    frog_on_me = TRUE;
}

// Sleep for certain amount of milliseconds, handling interrupts
void msleep(time_t timer) {
    for(int dec = 0; dec < MSLEEP_INTEVAL; dec++) {
        usleep(timer * MSEC_IN_SEC / MSLEEP_INTEVAL);
    }
}