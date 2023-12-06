// Include libs
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

// Play a manche, return remaining time of the manche or a manche code
int play_manche(bool* holes_occupied, int* n_lifes) {
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

    // Init control vars
    bool manche_ended = FALSE;
    int time_remaining = TIME_MANCHE;
    Message msg; // Define msg to store pipe message
    Frog frog = {INIT_FROG_Y, INIT_FROG_X};

    // Background color to restore
    int int_restore_color;
    attr_t restore_color;
    int frog_restore_colors[FROG_Y_DIM];

    print_bg_frog();
    print_lifes(*n_lifes);
    wrefresh(main_scr);

    while(!manche_ended) {

        read(pipe_fds[PIPE_READ], &msg, sizeof(Message));

        switch(msg.id) {
            case FROG_ID:
                // Restore old frog position
                if(frog.y < LINE_RIVER) {restore_color = GREEN_PURPLE;}
                else if(frog.y < LINE_BANK_2) {restore_color = GREEN_BLUE;}
                else {restore_color = GREEN_PURPLE;}
                for(i = frog.y; i - frog.y < FROG_Y_DIM; i++) {
                    mvwaprintw(main_scr, i, frog.x, restore_color, "%*c", FROG_X_DIM, ' ');
                }

                if(frog.y >= LIM_UP && frog.y <= LIM_DOWN) { // If frog can move...
                    frog.y += msg.y; // Update coordinate
                    if(frog.y < LIM_UP) { // If frog is outside limit...
                        frog.y = LIM_UP; // Move to limit
                    } else if(frog.y > LIM_DOWN) { // If frog is outside limit...
                        frog.y = LIM_DOWN; // Move to limit
                    }
                }
                if(frog.x >= LIM_LEFT && frog.x <= LIM_RIGHT) { // If frog can move...
                    frog.x += msg.x; // Update coordinate
                    if(frog.x < LIM_LEFT) { // If frog is outside limit...
                        frog.x = LIM_LEFT; // Move to limit
                    } else if(frog.x > LIM_RIGHT) { // If frog is outside limit...
                        frog.x = LIM_RIGHT; // Move to limit
                    }
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

            case TIME_ID:
                print_time(time_remaining, msg.x);
                if(time_remaining <= 0) {
                    manche_ended = TRUE;
                } else {
                    time_remaining--;
                }
                break;

            case PAUSE_ID:
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

            case CLOSE_ID:
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

        // Collisions ***

        wrefresh(main_scr);
    }
    signal_all(process_pids, SIGKILL);
    return time_remaining;
}
