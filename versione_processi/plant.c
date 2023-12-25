// Include libs
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "plant.h"

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