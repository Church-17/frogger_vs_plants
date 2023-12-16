// Include libs
#include "../game.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "time.h"

// Define constant
#define FLEX_INTERVAL 1

void time_process(int pipe_write, int* other_params) {
    // Init vars
    time_t start = timestamp(), end = start, elapsed;
    Message msg;
    msg.id = TIME_ID;
    msg.y = TIME_MANCHE;

    // Loop for beat time
    while(TRUE) {
        elapsed = end - start; // Calc elapsed milliseconds
        if(elapsed == MSEC_IN_SEC - FLEX_INTERVAL) elapsed++; // Give flexibility of an interval
        if(elapsed >= MSEC_IN_SEC) { // If a seconds passed...
            start = end; // Update start
            msg.y--;
            writer(pipe_write, &msg); // Write in pipe
            elapsed -= MSEC_IN_SEC; // Decrese elapsed of a second
        }
        if(elapsed < MSEC_IN_SEC) { // If elasped is now less that a second
            msleep(MSEC_IN_SEC - elapsed); // Wait for the milliseconds left to get to a second
        }
        end = timestamp(); // Update end
    }
}