#include "process.h"
#include "time.h"
#include "../struct.h"

#define SECOND 1

void time_process(int pipe_write) {
    Message msg;
    msg.sig = SIG_TIME;
    msg.cmd = TIME_MANCHE;
    while(TRUE) {
        write(pipe_write, &msg, sizeof(Message));
        sleep(SECOND);
        (msg.cmd)--; 
    }
}