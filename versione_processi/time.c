// Include libs
#include <time.h>
#include "../struct.h"
#include "../manche.h"
#include "process.h"
#include "time.h"

// Define constant
#define INTERVAL_CHECK 500000

void time_process(int pipe_write) {
    time_t start = 0, end;
    Message msg;
    msg.id = TIME_ID;
    while(TRUE) {
        end = time(NULL);
        if(end - start > 0) {
            writer(pipe_write, &msg);
            start = end;
        }
        usleep(INTERVAL_CHECK);
    }
}