// Include libs
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "frog.h"

// Frog function, called by a new process
void frog_process(int pipe_write, int* other_params) {
    // Init vars
    bool do_send_msg = FALSE;
    int key;
    Message msg;
    msg.id = FROG_ID;
    msg.sig = FROG_POSITION_SIG;

    // Init window from which to take input
    WINDOW* input_win = new_win(1, 1, 0, 0);

    // Frog loop to get pressed key
    while(TRUE) {
        key = wgetch(input_win);
        switch(key) {
            case 'w':
            case 'w' - CAPITAL_SHIFT:
            case KEY_UP:
                msg.y = -FROG_MOVE_Y;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send msg
                break;
                
            case 's':
            case 's' - CAPITAL_SHIFT:
            case KEY_DOWN:
                msg.y = FROG_MOVE_Y;
                msg.x = 0;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case 'a':
            case 'a' - CAPITAL_SHIFT:
            case KEY_LEFT:
                msg.y = 0;
                msg.x = -FROG_MOVE_X;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case 'd':
            case 'd' - CAPITAL_SHIFT:
            case KEY_RIGHT:
                msg.y = 0;
                msg.x = FROG_MOVE_X;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case PAUSE_GAME_KEY:
            case PAUSE_GAME_KEY - CAPITAL_SHIFT:
                msg.id = PAUSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case CLOSE_GAME_KEY:
            case CLOSE_GAME_KEY - CAPITAL_SHIFT:
                msg.id = CLOSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case SHOT_GAME_KEY:
                msg.sig = FROG_SHOT_SIG;
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
            msg.sig = FROG_POSITION_SIG;
            do_send_msg = FALSE;
        }
    }
}