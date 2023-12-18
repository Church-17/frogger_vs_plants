// Include libs
#include "../game.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "time.h"

void time_process(int pipe_write, int* other_params) {
    // Init vars
    time_t start = timestamp(), end = start, elapsed;
    Message msg;
    msg.id = TIME_ID;
    msg.y = TIME_MANCHE;

    // Loop for beat time
    while(TRUE) {
        elapsed = end - start; // Calc elapsed milliseconds
        while(elapsed >= MSEC_IN_SEC) { // If a seconds passed...
            start = end; // Update start
            msg.y--;
            writer(pipe_write, &msg); // Write in pipe
            elapsed -= MSEC_IN_SEC; // Decrese elapsed of a second
        }
        msleep(MSEC_IN_SEC - elapsed); // Wait for the milliseconds left to get to a second
        end = timestamp(); // Update end
    }
}