#include "../struct.h"
#include "../manche.h"
#include "process.h"
#include "time.h"

void time_process(int pipe_write) {
    int time_remaining = TIME_MANCHE;
    double char_in_sec_timebar = (double) TIMEBAR_LEN / TIME_MANCHE;
    Message msg;
    msg.id = TIME_ID;
    msg.x = TIMEBAR_LEN;
    while(TRUE) {
        write(pipe_write, &msg, sizeof(Message));
        time_remaining--;
        msg.x = time_remaining * char_in_sec_timebar;
        sleep(1);
    }
}