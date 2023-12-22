// Include libs
#include "../sprites.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "bullet.h"

void bullet_process(int pipe_write, int* other_params) {
    bool do_exit = FALSE;
    Message msg;

    msg.id = other_params[BULLET_ID_INDEX];
    msg.y = other_params[BULLET_Y_INDEX];
    msg.x = other_params[BULLET_X_INDEX];

    writer(pipe_write, &msg);

    while(!do_exit) {
        if(msg.id < MIN_PLANT_BULLET_ID) {
            msg.y -= MOVE_BULLET_Y;
            if(msg.y < LINE_BANK_1) {
                do_exit = TRUE;
            }
        } else {
            msg.y += MOVE_BULLET_Y;
            if(msg.y >= MAIN_ROWS) {
                do_exit = TRUE;
            }
        }

        msleep(MSEC_IN_SEC / BULLET_SPEED);
        writer(pipe_write, &msg);
    }

}