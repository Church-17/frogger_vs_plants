// Include libs
#include "../main.h"
#include "../game.h"
#include "../struct.h"
#include "process.h"
#include "frog.h"

// Frog function, called by a new process
void frog_process(int pipe_write) {
    // Init vars
    bool do_send_msg = FALSE;
    int key;

    // Initial position
    Message msg;
    msg.id = FROG_ID;

    // Frog loop to get pressed key
    while(TRUE) {
        key = wgetch(main_scr);
        switch(key) {
            case KEY_UP:
                msg.y = -MOVE_FROG_Y;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send msg
                break;
                
            case KEY_DOWN:
                msg.y = MOVE_FROG_Y;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case KEY_LEFT:
                msg.y = 0;
                msg.x = -MOVE_FROG_X;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case KEY_RIGHT:
                msg.y = 0;
                msg.x = MOVE_FROG_X;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case PAUSE_GAME_KEY:
                msg.id = PAUSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case CLOSE_GAME_KEY:
                msg.id = CLOSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case KEY_RESIZE:
                msg.id = RESIZE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            default: break; 
        }
        if(do_send_msg) {
            writer(pipe_write, &msg);
            msg.id = FROG_ID;
            do_send_msg = FALSE;
        }
    }
}