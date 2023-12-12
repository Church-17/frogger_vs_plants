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
    // Init vars
    int n_stream, speed_stream, limit;
    Message msg;

    // Unpack croccodile params
    msg.id = other_params[0];
    n_stream = other_params[1];
    speed_stream = other_params[2];

    // Determine coordinates
    msg.y = LINE_RIVER + FROG_DIM_Y * n_stream;

    if (speed_stream > 0) { // If the stream direction is from L to R
        msg.x = -CROCCODILE_DIM_X + MOVE_CROCCODILE_X;
        limit = MAIN_COLS;
    } else { // If the stream direction is from R to L
        msg.x = MAIN_COLS - MOVE_CROCCODILE_X;
        limit = -CROCCODILE_DIM_X;
    }

    // Random spawn time
    msleep(rand_range(1, 3) * MSEC_IN_SEC);

    // Loop for write new coordinates
    while(TRUE) {
        // Write on pipe
        writer(pipe_write, &msg);

        // Update X coordinate
        if (limit == MAIN_COLS) {
            if (msg.x < MAIN_COLS) {
                msg.x += MOVE_CROCCODILE_X;
            }
        } else {
            if (msg.x > -CROCCODILE_DIM_X) {
                msg.x -= MOVE_CROCCODILE_X;
            }
        }

        // Sleep based on speed
        msleep(speed_stream > 0 ? speed_stream : -speed_stream);
    }
}