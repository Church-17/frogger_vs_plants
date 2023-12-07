#include "../struct.h"
#include "../manche.h"
#include "process.h"
#include "time.h"

void time_process(int pipe_write) {
    int time_remaining = TIME_MANCHE;
    Message msg;
    msg.id = TIME_ID;
    while(TRUE) {
        writer(pipe_write, msg);
        time_remaining--;
        sleep(1);
    }
}