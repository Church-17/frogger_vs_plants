// Include libs
#include "../main.h"
#include "../menu.h"
#include "../str.h"
#include "../sprites.h"
#include "../game.h"
#include "../manche.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "frog.h"

// Define constant
#define LIM_N_PROCESS 30

// Play a manche, return remaining time of the manche or a Manche_Index
int play_manche(bool* holes_occupied, int n_lifes) {
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

    process_pids.list[SIG_FROG] = forker(&process_pids); // Calls the fork handling the errors
    (process_pids.len)++;
    if(process_pids.list[SIG_FROG] == PID_CHILD) {
        close(pipe_fds[PIPE_READ]);
        frog_process(pipe_fds[PIPE_WRITE]);
        _exit(ERR_FORK); // Handle unexpected process termination
    }
    
    close(pipe_fds[PIPE_WRITE]); // Close unused fd

    // Colors under frog per line
    int int_restore_color;
    attr_t restore_color;
    int frog_restore_colors[FROG_Y_DIM] = {COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE};

    Message msg; // Define msg to store pipe message
    Frog frog = {INIT_FROG_Y, INIT_FROG_X};

    print_bg_frog();
    wrefresh(main_scr);

    while(TRUE) {

        read(pipe_fds[PIPE_READ], &msg, sizeof(Message));

        switch(msg.sig) {
            case SIG_FROG:
                // Restore old frog position
                if(frog.y < LINE_RIVER) {restore_color = GREEN_PURPLE;}
                else if(frog.y < LINE_BANK_2) {restore_color = GREEN_BLUE;}
                else {restore_color = GREEN_PURPLE;}
                for(i = frog.y; i - frog.y < FROG_Y_DIM; i++) {
                    mvwaprintw(main_scr, i, frog.x, restore_color, "%*c", FROG_X_DIM, ' ');
                }
                switch(msg.cmd) {
                    case MOVE_UP:
                        if(frog.y > LIM_UP) { // If frog can move...
                            frog.y -= MOVE_FROG_Y; // Update coordinate
                            if(frog.y < LIM_UP) { // If frog is outside limit...
                                frog.y = LIM_UP; // Move to limit
                            }
                        }
                        break;

                    case MOVE_DOWN:
                        if(frog.y < LIM_DOWN) { // If frog can move...
                            frog.y += MOVE_FROG_Y; // Update coordinate
                            if(frog.y > LIM_DOWN) { // If frog is outside limit...
                                frog.y = LIM_DOWN; // Move to limit
                            }
                        }
                        break;

                    case MOVE_LEFT:
                        if(frog.x > LIM_LEFT) { // If frog can move...
                            frog.x -= MOVE_FROG_X; // Update coordinate
                            if(frog.x < LIM_LEFT) { // If frog is outside limit...
                                frog.x = LIM_LEFT; // Move to limit
                            }
                        }
                        break;

                    case MOVE_RIGHT:
                        if(frog.x < LIM_RIGHT) { // If frog can move...
                            frog.x += MOVE_FROG_X; // Update coordinate
                            if(frog.x > LIM_RIGHT) { // If frog is outside limit...
                                frog.x = LIM_RIGHT; // Move to limit
                            }
                        }
                        break;
                }
                // Pick frog background
                if(frog.y < LINE_RIVER) {int_restore_color = COLOR_PURPLE;}
                else if(frog.y < LINE_BANK_2) {int_restore_color = COLOR_BLUE;}
                else {int_restore_color = COLOR_PURPLE;}
                for(i = 0; i < FROG_Y_DIM; i++) {
                    frog_restore_colors[i] = int_restore_color;
                }
                print_frog(main_scr, frog.y, frog.x, frog_restore_colors);
                break;

            case SIG_PAUSE:
                signal_all(process_pids, SIGSTOP);
                i = pause_menu();
                switch (i) {
                    case PAUSE_RES_ID:
                        break;
                    
                    case PAUSE_RETR_ID:
                        return MANCHE_RETR;

                    case PAUSE_BACK_ID:
                        return MANCHE_CLOSE;

                    case PAUSE_QUIT_ID:
                        return MANCHE_QUIT;
                }
                redrawwin(main_scr);
                signal_all(process_pids, SIGCONT);
                break;

            case SIG_CLOSE:
                signal_all(process_pids, SIGSTOP);
                i = quit_menu();    
                switch (i) {
                    case NO_ID:
                        break;
                    
                    case YES_ID:
                        return MANCHE_CLOSE;
                }
                redrawwin(main_scr);
                signal_all(process_pids, SIGCONT);
                break;
        }
        wrefresh(main_scr);
    }
    return 0;
}