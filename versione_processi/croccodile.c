// Include libs
#include "../main.h"
#include "../game.h"
#include "../struct.h"
#include "../utils.h"
#include "../sprites.h"
#include "process.h"
#include "croccodile.h"

// params: [communication_id, n_stream, speed_stream]
void croccodile_process(int pipe_write, int* other_params) {
    srand(timestamp());

    // Init vars
    bool do_exit = FALSE;
    int n_stream, speed_stream, limit;
    Message msg;

    // Unpack croccodile params
    msg.id = other_params[0];
    n_stream = other_params[1];
    speed_stream = other_params[2];

    // Determine coordinates
    msg.y = LINE_RIVER + FROG_DIM_Y * n_stream;

    if(speed_stream > 0) { // If the stream direction is from L to R
        msg.x = -CROCCODILE_DIM_X + MOVE_CROCCODILE_X;
        limit = MAIN_COLS;
    } else { // If the stream direction is from R to L
        msg.x = MAIN_COLS - MOVE_CROCCODILE_X;
        limit = -CROCCODILE_DIM_X;
    }

    // Random spawn time
    msleep(rand_range(1, 5) * MSEC_IN_SEC);
    writer(pipe_write, &msg);

    // Loop for write new coordinates
    while(!do_exit) {
        // Update X coordinate
        if(speed_stream > 0) {
            msg.x += MOVE_CROCCODILE_X;
            if(msg.x >= limit) {
                do_exit = TRUE;
            }
        } else {
            msg.x -= MOVE_CROCCODILE_X;
            if(msg.x <= limit) {
                do_exit = TRUE;
            }
        }

        // Sleep based on speed
        msleep(speed_stream > 0 ? speed_stream : -speed_stream);

        // Write on pipe
        writer(pipe_write, &msg);
    }
    return;
}