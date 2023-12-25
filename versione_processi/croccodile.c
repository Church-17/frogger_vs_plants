// Include libs
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "croccodile.h"

// Global vars
bool frog_on_me = FALSE;

// Function prototypes
void frog_stepped_on_me(int sig);

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
    msg.x = speed_stream > 0 ? -CROCCODILE_DIM_X + MOVE_CROCCODILE_X : MAIN_COLS - MOVE_CROCCODILE_X;

    // Random croccodile kind & spawn time
    srand(timestamp() + msg.id);
    msg.sig = rand_range(0, 10) < BAD_THRESHOLD ? BAD_CROCCODILE_SIG : GOOD_CROCCODILE_SIG;
    msleep(rand_range(MIN_CROCCODILE_SPAWN_TIME, MAX_CROCCODILE_SPAWN_TIME) * MSEC_IN_SEC);

    // Write initial position
    writer(pipe_write, &msg);

    // Loop for write new coordinates
    while(!do_exit) {
        // Update X coordinate
        if(speed_stream > 0) {
            msg.x += MOVE_CROCCODILE_X;
            if(msg.x >= MAIN_COLS) {
                do_exit = TRUE;
            }
        } else {
            msg.x -= MOVE_CROCCODILE_X;
            if(msg.x <= -CROCCODILE_DIM_X) {
                do_exit = TRUE;
            }
        }

        if(frog_on_me && msg.sig >= BAD_CROCCODILE_SIG) { // If croccodile is bad and frog stepped on...
            if(!do_immersion) { // If immersion not started, start it
                immersion_time = rand_range(2, 4) * MSEC_IN_SEC;
                start = timestamp();
                do_immersion = TRUE;
            }
            end = timestamp();
            if(end - start >= immersion_time) { // If the croccodile have to immerge...
                msg.sig = IMMERSION_CROCCODILE_SIG;
                do_exit = TRUE;
            } else if(end - start >= immersion_time - BUBBLE_THRESHOLD) { // If there is BUBBLE_THRESHOLD left to immersion...
                msg.sig = BUBBLE_CROCCODILE_SIG;
            }
        }

        msleep(MSEC_IN_SEC / (speed_stream > 0 ? speed_stream : -speed_stream)); // Sleep based on speed
        writer(pipe_write, &msg); // Write on pipe
    }
    return;
}

void frog_stepped_on_me(int sig) { // If frog steps on this croccodile
    frog_on_me = TRUE;
}