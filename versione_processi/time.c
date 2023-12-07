#include "../struct.h"
#include "../manche.h"
#include "process.h"
#include "time.h"
#include <time.h>

void time_process(int pipe_write) {
    time_t start = 0, end;
    Message msg;
    msg.id = TIME_ID;
    while(TRUE) {
        end = time(NULL);
        if(end - start > 0) {
            writer(pipe_write, msg);
            start = end;
        }
        usleep(500000);
    }
}