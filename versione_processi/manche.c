// Include libs
#include <time.h>
#include "../menu.h"
#include "../str.h"
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "frog.h"
#include "time.h"

// Define constant
#define LIM_N_PROCESS 30

// Play a manche, return game vars
Game_t play_manche(bool* holes_occupied, int n_lifes) {
    // Erase old game or menu_bg
    wclear(main_scr);
    wrefresh(main_scr);

    // Init vars
    int i;
    pid_t array_pids[LIM_N_PROCESS]; // Pids for every process
    List_pid process_pids;
    process_pids.list = array_pids;
    process_pids.len = 0;
    
    // Pipe
    int pipe_fds[PIPE_DIM];
    piper(pipe_fds); // Starts the pipe handling the errors

    forker(FROG_ID, &process_pids, frog_process, pipe_fds); // Calls the fork for frog process handling the errors

    forker(TIME_ID, &process_pids, time_process, pipe_fds); // Calls the fork for time process handling the errors
    
    // --- PARENT PROCESS ---

    close(pipe_fds[PIPE_WRITE]); // Close unused fd

    // Background color to restore
    attr_t restore_color;

    // Init control vars
    bool manche_ended = FALSE;
    int resize_time = 0;
    Message msg; // Define msg to store pipe message

    // Init game vars
    Game_t gamevar;
    gamevar.frog.y = INIT_FROG_Y;
    gamevar.frog.x = INIT_FROG_X;
    gamevar.timer = TIME_MANCHE;
    gamevar.lifes = n_lifes;
    gamevar.holes_occupied = holes_occupied;

    print_game(&gamevar);
    wrefresh(main_scr);

    while(!manche_ended) {

        read(pipe_fds[PIPE_READ], &msg, sizeof(Message));

        switch(msg.id) {
            case FROG_ID:
                // Restore old frog position
                if(gamevar.frog.y < LINE_RIVER) {restore_color = GREEN_PURPLE;}
                else if(gamevar.frog.y < LINE_BANK_2) {restore_color = GREEN_BLUE;}
                else {restore_color = GREEN_PURPLE;}
                for(i = gamevar.frog.y; i - gamevar.frog.y < FROG_Y_DIM; i++) {
                    mvwaprintw(main_scr, i, gamevar.frog.x, restore_color, "%*s", FROG_X_DIM, "");
                }

                if(gamevar.frog.y >= LIM_UP && gamevar.frog.y <= LIM_DOWN) { // If frog can move...
                    gamevar.frog.y += msg.y; // Update coordinate
                    if(gamevar.frog.y < LIM_UP) { // If frog is outside limit...
                        gamevar.frog.y = LIM_UP; // Move to limit
                    } else if(gamevar.frog.y > LIM_DOWN) { // If frog is outside limit...
                        gamevar.frog.y = LIM_DOWN; // Move to limit
                    }
                }
                if(gamevar.frog.x >= LIM_LEFT && gamevar.frog.x <= LIM_RIGHT) { // If frog can move...
                    gamevar.frog.x += msg.x; // Update coordinate
                    if(gamevar.frog.x < LIM_LEFT) { // If frog is outside limit...
                        gamevar.frog.x = LIM_LEFT; // Move to limit
                    } else if(gamevar.frog.x > LIM_RIGHT) { // If frog is outside limit...
                        gamevar.frog.x = LIM_RIGHT; // Move to limit
                    }
                }
                print_frog(&gamevar);
                break;

            case TIME_ID:
                if(gamevar.timer <= 0) {
                    manche_ended = TRUE;
                } else {
                    gamevar.timer--;
                }
                print_time(gamevar.timer, FALSE);
                break;

            case RESIZE_ID:
                if(time(NULL) - resize_time < 1) {
                    break;
                }
                resize_proc(NULL, 0, 0, &gamevar);
            case PAUSE_ID:
                signal_all(process_pids, SIGSTOP);
                i = pause_menu(&gamevar);
                switch (i) {
                    case PAUSE_RES_ID:
                        break;
                    
                    case PAUSE_RETR_ID:
                        gamevar.timer = MANCHE_RETR;
                        return gamevar;

                    case PAUSE_BACK_ID:
                        gamevar.timer = MANCHE_CLOSE;
                        return gamevar;

                    case PAUSE_QUIT_ID:
                        gamevar.timer = MANCHE_QUIT;
                        return gamevar;
                }
                redrawwin(main_scr);
                signal_all(process_pids, SIGCONT);
                resize_time = time(NULL);
                break;

            case CLOSE_ID:
                signal_all(process_pids, SIGSTOP);
                i = quit_menu(&gamevar);    
                switch (i) {
                    case NO_ID:
                        break;
                    
                    case YES_ID:
                        gamevar.timer = MANCHE_CLOSE;
                        return gamevar;
                }
                redrawwin(main_scr);
                signal_all(process_pids, SIGCONT);
                break;
        }

        // Collisions ***

        wrefresh(main_scr);
    }
    signal_all(process_pids, SIGKILL);
    return gamevar;
}
