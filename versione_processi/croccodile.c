// Include libs
#include "../sprites.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "croccodile.h"

void frog_on(int sig);

bool frog_on_me = FALSE;

// params: [communication_id, n_stream, speed_stream]
void croccodile_process(int pipe_write, int* other_params) {
    // Init vars
    bool do_exit = FALSE, do_immersion = FALSE;
    int n_stream, speed_stream, immersion_time;
    time_t start;
    Message msg;

    signal(SIGUSR1, &frog_on);

    // Unpack croccodile params
    msg.id = other_params[CROCCODILE_ID_INDEX];
    n_stream = other_params[CROCCODILE_STREAM_INDEX];
    speed_stream = other_params[CROCCODILE_SPEED_INDEX];

    // Determine coordinates
    msg.y = LINE_RIVER + FROG_DIM_Y * n_stream;
    msg.x = speed_stream > 0 ? -CROCCODILE_DIM_X + MOVE_CROCCODILE_X : MAIN_COLS - MOVE_CROCCODILE_X;

    // Random seed
    srand(timestamp() + msg.id);

    // Random croccodile kind
    msg.y += rand_range(0, 10) < BAD_THRESHOLD ? BAD_CROCCODILE_OFFSET : 0;

    // Random spawn time
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

        if(frog_on_me && msg.y >= BAD_CROCCODILE_OFFSET) {
            if(!do_immersion) {
                immersion_time = rand_range(2, 4) * MSEC_IN_SEC;
                start = timestamp();
                do_immersion = TRUE;
            } else {
                if(timestamp() - start >= immersion_time) {
                    msg.x = -CROCCODILE_DIM_X;
                    do_exit = TRUE;
                }
            }
        }

        // Sleep based on speed
        msleep(MSEC_IN_SEC / (speed_stream > 0 ? speed_stream : -speed_stream));

        // Write on pipe
        writer(pipe_write, &msg);
    }
    return;
}

void frog_on(int sig) {
    frog_on_me = TRUE;
}