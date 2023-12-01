#include "../struct.h"
#include "../utils.h"
#include "../main.h"
#include "../sprites.h"
#include "../game.h"
#include "frog.h"
#include "process.h"

#define MOVE_Y 4
#define MOVE_X 2
#define LIM_UP 0
#define LIM_DOWN (MAIN_ROWS-FROG_Y_DIM-1)
#define LIM_LEFT 0
#define LIM_RIGHT (MAIN_COLS-FROG_X_DIM-1)

void frog(int pipe_write) {
    bool do_send_msg = FALSE;
    int key;

    // Initial position
    Message msg = {FROG_ID, LIM_DOWN, MAIN_COLS/2-FROG_X_DIM/2};
    write(pipe_write, &msg, sizeof(Message));

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

            case CLOSE_GAME:
                msg.id = CLOSE_ID;
                write(pipe_write, &msg, sizeof(Message));
                msg.id = FROG_ID;
                break;

            default: break; 
        }
        if(do_send_msg) {
            write(pipe_write, &msg, sizeof(Message));
            do_send_msg = FALSE;
        }
    }
}