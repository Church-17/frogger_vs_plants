// Include libs
#include "../main.h"
#include "../game.h"
#include "../struct.h"
#include "process.h"
#include "frog.h"

// Frog function, called by a new process
void frog_process(WINDOW* game_scr, int pipe_write, int frog_id, int* other_params) {
    // Init vars
    bool do_send_msg = FALSE;
    int key;

    // Initial position
    Message msg;
    msg.id = frog_id;

    // Frog loop to get pressed key
    while(TRUE) {
        key = wgetch(game_scr);
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
            msg.id = frog_id;
            do_send_msg = FALSE;
        }
    }
}