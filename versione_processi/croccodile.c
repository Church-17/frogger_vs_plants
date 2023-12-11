// Include libs
#include "../main.h"
#include "../game.h"
#include "../struct.h"
#include "../utils.h"
#include "../sprites.h"
#include "process.h"
#include "croccodile.h"

// params: [n_stream, speed_stream]
void croccodile_process(WINDOW* game_scr, int pipe_write, int croccodile_id, int* other_params) {


    // Initial position
    Message msg;
    msg.id = croccodile_id;

    int n_stream, speed_stream, t_spawn, limit;

    n_stream = other_params[0];
    speed_stream = other_params[1];

    msleep(rand_range(0, 3) * MSEC_IN_SEC); // wait before spawn

    msg.y = LINE_RIVER + FROG_DIM_Y * n_stream;
    
    if (speed_stream > 0) {
        msg.x = -CROCCODILE_DIM_X + MOVE_CROCCODILE_X;
        limit = MAIN_COLS;
    } else {
        msg.x = MAIN_COLS - MOVE_CROCCODILE_X;
        limit = -CROCCODILE_DIM_X;
    }

    // Frog loop to get pressed key
    while(TRUE) {
        
        writer(pipe_write, &msg);

        if (limit == MAIN_COLS) {
            if (msg.x < MAIN_COLS) {
                msg.x += MOVE_CROCCODILE_X;
            }
        } else {
            if (msg.x > -CROCCODILE_DIM_X) {
                msg.x -= MOVE_CROCCODILE_X;
            }
        }
            
        msleep(speed_stream > 0 ? speed_stream : -speed_stream);

    }

}