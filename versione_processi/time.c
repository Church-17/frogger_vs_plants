#include "../struct.h"
#include "../manche.h"
#include "process.h"
#include "time.h"

void time_process(int pipe_write) {
    int time_remaining = TIME_MANCHE;
    double sec_in_char_timebar = TIME_MANCHE / TIMEBAR_LEN;
    Message msg;
    msg.sig = SIG_TIME;
    msg.cmd = TIMEBAR_LEN;
    while(TRUE) {
        write(pipe_write, &msg, sizeof(Message));
        (time_remaining)--;
        msg.cmd = time_remaining / sec_in_char_timebar + 1;
        sleep(1);
    }
}