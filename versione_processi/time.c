// Include libs
#include "../struct.h"
#include "../utils.h"
#include "../manche.h"
#include "process.h"
#include "time.h"

void time_process(int pipe_write) {
    time_t start = timestamp(), end = start, elapsed;
    Message msg;
    msg.id = TIME_ID;
    while(TRUE) {
        elapsed = end - start;
        if(elapsed >= MSEC_IN_SEC) {
            start = end;
            writer(pipe_write, &msg);
            elapsed -= MSEC_IN_SEC;
        }
        if(elapsed < MSEC_IN_SEC) {
            usleep((MSEC_IN_SEC - elapsed) * USEC_IN_MSEC);
        }
        end = timestamp();
    }
}