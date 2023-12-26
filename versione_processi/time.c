// Include libs
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "time.h"

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