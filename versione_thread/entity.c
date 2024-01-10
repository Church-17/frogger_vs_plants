// Include libs
#include <unistd.h>
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "thread.h"
#include "entity.h"

// Define contants
#define CROCCODILE_NOT_SHOTTED (-1)

// Global vars
int frog_on_croccodile = FROG_NOT_ON_CROCCODILE, croccodile_shotted = CROCCODILE_NOT_SHOTTED;
pthread_mutex_t shotted_mutex = PTHREAD_MUTEX_INITIALIZER;

void* time_thread(void* params) {
    // Init vars
    time_t start = timestamp(), end = start;
    Message msg;
    msg.id = TIME_ID;
    msg.sig = TIME_MANCHE;

    // Loop for beat time
    while(TRUE) {
        if(end - start >= MSEC_IN_SEC) { // If a seconds passed...
            start = end; // Update start
            msg.sig--; // Decrease game timer
            write_msg(msg);
        }
        msleep(MSEC_IN_SEC); // Sleep for a second
        end = timestamp(); // Update end
    }
}

void* frog_thread(void* params) {
    // Init vars
    bool do_send_msg = FALSE; // Flag
    int key, direction = 1;
    Message msg;
    msg.id = FROG_ID;
    msg.sig = FROG_POSITION_SIG;

    // Init window from which to take input
    WINDOW* input_win = new_win(1, 1, 0, 0);
    wtimeout(input_win, 100); // Chech pause every 0.1 second

    // Frog loop to get pressed key
    while(TRUE) {
        check_pause(); // Check if game is in pausepause
        key = wgetch(input_win);
        switch(key) {
            case MOVE_N_KEY:
            case MOVE_N_KEY - CAPITAL_SHIFT:
            case KEY_UP:
                direction = -1; // Invert direction
            case MOVE_S_KEY:
            case MOVE_S_KEY - CAPITAL_SHIFT:
            case KEY_DOWN:
                msg.y = FROG_MOVE_Y * direction;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send message
                break;

            case MOVE_W_KEY:
            case MOVE_W_KEY - CAPITAL_SHIFT:
            case KEY_LEFT:
                direction = -1; // Invert direction
            case MOVE_E_KEY:
            case MOVE_E_KEY - CAPITAL_SHIFT:
            case KEY_RIGHT:
                msg.y = 0;
                msg.x = FROG_MOVE_X * direction;
                do_send_msg = TRUE; // Set flag to send message
                break;

            case SHOT_GAME_KEY:
                msg.sig = FROG_SHOT_SIG; // Change message signal to shot a bullet
                do_send_msg = TRUE; // Set flag to send message
                break;

            case PAUSE_GAME_KEY:
            case PAUSE_GAME_KEY - CAPITAL_SHIFT:
                msg.id = PAUSE_ID; // Change ID to pass pause command
                do_send_msg = TRUE; // Set flag to send message
                break;

            case CLOSE_GAME_KEY:
            case CLOSE_GAME_KEY - CAPITAL_SHIFT:
                msg.id = CLOSE_ID; // Change ID to pass close command
                do_send_msg = TRUE; // Set flag to send message
                break;

            case KEY_RESIZE:
                msg.id = RESIZE_ID; // Change ID to do resize procedure
                do_send_msg = TRUE; // Set flag to send message
                break;

            default: break; 
        }
        if(do_send_msg) {
            write_msg(msg); // Write message on buffer
            // Restore defaults
            direction = 1;
            msg.id = FROG_ID;
            msg.sig = FROG_POSITION_SIG;
            do_send_msg = FALSE;
        }
    }
}

// params: [communication_id, n_stream, speed_stream]
void* croccodile_thread(void* params) {
    // Init vars
    bool do_exit = FALSE, do_immersion = FALSE;
    int n_stream, speed_stream, immersion_time;
    time_t start, end;
    Message msg;

    // Unpack croccodile params
    msg.id = ((int*) params)[CROCCODILE_ID_INDEX];
    n_stream = ((int*) params)[CROCCODILE_STREAM_INDEX];
    speed_stream = ((int*) params)[CROCCODILE_SPEED_INDEX];

    // Determine coordinates
    msg.y = LINE_RIVER + FROG_DIM_Y * n_stream;
    msg.x = speed_stream > 0 ? -CROCCODILE_DIM_X + CROCCODILE_MOVE_X : MAIN_COLS - CROCCODILE_MOVE_X;

    // Random croccodile kind & spawn time
    msg.sig = rand_range(0, 10) < BAD_THRESHOLD ? CROCCODILE_BAD_SIG : CROCCODILE_GOOD_SIG;
    msleep(rand_range(MIN_CROCCODILE_SPAWN_TIME, MAX_CROCCODILE_SPAWN_TIME) * MSEC_IN_SEC);

    write_msg(msg); // Write initial position

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

        // Check if this croccodile thread has been shotted
        if(croccodile_shotted == msg.id) {
            msg.sig = CROCCODILE_GOOD_SIG;
            change_croccodile_shotted(CROCCODILE_NOT_SHOTTED); // Reset croccodile shotted var
        }

        // Check if frog is on this croccodile thread
        if(frog_on_croccodile == msg.id && msg.sig != CROCCODILE_GOOD_SIG) { // If croccodile is bad and frog stepped on...
            if(!do_immersion) { // If immersion not started, start it
                immersion_time = rand_range(2, 4) * MSEC_IN_SEC; // Calc random immersion time
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
        write_msg(msg);
    }
    return NULL;
}

// params: [communication_id, x]
void* plant_thread(void* params) {
    // Init vars
    Message msg;

    // Unpack croccodile params
    msg.id = ((int*) params)[PLANT_ID_INDEX];
    msg.y = LINE_BANK_1;
    msg.x = ((int*) params)[PLANT_X_INDEX];
    msg.sig = PLANT_SPAWN_SIG;

    // Random spawn time & shot interval
    msleep(rand_range(1, 5) * MSEC_IN_SEC);

    write_msg(msg); // Write initial position

    msg.sig = PLANT_SHOT_SIG; // After spawn the plant only shot

    // Plant loop to shot bullets
    while(TRUE) {
        msleep(MSEC_IN_SEC*(PLANT_SHOT_INTERVAL + rand_range(0, 3)));
        write_msg(msg);
    }
}

// params: [communication_id, y, x]
void* bullet_thread(void* params) {
    // Init vars
    bool do_exit = FALSE;
    Message msg;

    // Unpack bullet params
    msg.id = ((int*) params)[BULLET_ID_INDEX];
    msg.y = ((int*) params)[BULLET_Y_INDEX];
    msg.x = ((int*) params)[BULLET_X_INDEX];

    // Write initial position
    write_msg(msg);
    if(msg.y < LINE_BANK_1) {
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
        write_msg(msg);
    }
    return NULL;
}

// Change ID of croccodile shotted
void change_croccodile_shotted(int id) {
    if(pthread_mutex_lock(&shotted_mutex) != 0) { // Handle mutex lock error
        quit(ERR_MUTEX_LOCK);
    }
    croccodile_shotted = id;
    if(pthread_mutex_unlock(&shotted_mutex) != 0) { // Handle mutex unlock error
        quit(ERR_MUTEX_UNLOCK);
    }
}