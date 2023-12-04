// Include libs
#include "../struct.h"
#include "../utils.h"
#include "../main.h"
#include "../sprites.h"
#include "../game.h"
#include "frog.h"
#include "process.h"

// Define constant
#define MOVE_Y 4
#define MOVE_X 2
#define LIM_UP 0
#define LIM_DOWN (MAIN_ROWS-FROG_Y_DIM)
#define LIM_LEFT 0
#define LIM_RIGHT (MAIN_COLS-FROG_X_DIM)

// Frog function, called by a new process
void frog(int pipe_write) {
    // Init vars
    bool do_send_msg = FALSE;
    int key;

    // Initial position
    Message msg = {FROG_ID, LIM_DOWN, MAIN_COLS/2-FROG_X_DIM/2};
    write(pipe_write, &msg, sizeof(Message));

    // Frog loop to get pressed key
    while(TRUE) {
        key = wgetch(main_scr);
        switch(key) {
            case KEY_UP:
                if(msg.y > LIM_UP) { // If frog can move...
                    do_send_msg = TRUE; // Set flag to send msg
                    msg.y -= MOVE_Y; // Update coordinate
                    if(msg.y < LIM_UP) { // If frog is outside limit...
                        msg.y = LIM_UP; // Move to limit
                    }
                }
                break;
                
            case KEY_DOWN:
                if(msg.y < LIM_DOWN) { // If frog can move...
                    do_send_msg = TRUE; // Set flag to send msg
                    msg.y += MOVE_Y; // Update coordinate
                    if(msg.y > LIM_DOWN) { // If frog is outside limit...
                        msg.y = LIM_DOWN; // Move to limit
                    }
                }
                break;

            case KEY_LEFT:
                if(msg.x > LIM_LEFT) { // If frog can move...
                    do_send_msg = TRUE; // Set flag to send msg
                    msg.x -= MOVE_X; // Update coordinate
                    if(msg.x < LIM_LEFT) { // If frog is outside limit...
                        msg.x = LIM_LEFT; // Move to limit
                    }
                }
                break;

            case KEY_RIGHT:
                if(msg.x < LIM_RIGHT) { // If frog can move...
                    do_send_msg = TRUE; // Set flag to send msg
                    msg.x += MOVE_X; // Update coordinate
                    if(msg.x > LIM_RIGHT) { // If frog is outside limit...
                        msg.x = LIM_RIGHT; // Move to limit
                    }
                }
                break;

            case PAUSE_GAME_KEY:
                msg.id = PAUSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            case CLOSE_GAME_KEY:
                msg.id = CLOSE_ID;
                do_send_msg = TRUE; // Set flag to send msg
                break;

            default: break; 
        }
        if(do_send_msg) {
            write(pipe_write, &msg, sizeof(Message));
            msg.id = FROG_ID;
            do_send_msg = FALSE;
        }
    }
}