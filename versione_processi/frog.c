// Include libs
#include "../struct.h"
#include "../utils.h"
#include "../main.h"
#include "../sprites.h"
#include "../game.h"
#include "frog.h"
#include "process.h"

// Frog function, called by a new process
void frog_process(int pipe_write) {
    // Init vars
    bool do_send_msg = FALSE;
    int key;

    // Initial position
    Message msg;
    msg.sig = SIG_FROG;

    // Frog loop to get pressed key
    while(TRUE) {
        key = wgetch(main_scr);
        switch(key) {
            case KEY_UP:
                msg.cmd = MOVE_UP;
                do_send_msg = TRUE; // Set flag to send msg
                break;
                
            case KEY_DOWN:
                msg.cmd = MOVE_DOWN;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case KEY_LEFT:
                msg.cmd = MOVE_LEFT;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case KEY_RIGHT:
                msg.cmd = MOVE_RIGHT;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case PAUSE_GAME_KEY:
                msg.sig = SIG_PAUSE;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case CLOSE_GAME_KEY:
                msg.sig = SIG_CLOSE;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            default: break; 
        }
        if(do_send_msg) {
            write(pipe_write, &msg, sizeof(Message));
            do_send_msg = FALSE;
        }
    }
}