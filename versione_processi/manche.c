// Include libs
#include "../main.h"
#include "../menu.h"
#include "../str.h"
#include "../sprites.h"
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

    process_pids.list[FROG_ID] = forker(&process_pids); // Calls the fork handling the errors
    (process_pids.len)++;
    if(process_pids.list[FROG_ID] == PID_CHILD) {
        close(pipe_fds[PIPE_READ]);
        frog(pipe_fds[PIPE_WRITE]);
        _exit(ERR_FORK); // Handle unexpected process termination
    }
    
    close(pipe_fds[PIPE_WRITE]); // Close unused fd

    // Store old coordinates
    int old_frog_y = MAIN_ROWS/2, old_frog_x = MAIN_COLS/2;

    // Colors under frog per line
    int frog_restore_colors[FROG_Y_DIM] = {COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK};

    Message msg; // Define msg to store pipe message

    while(TRUE) {

        read(pipe_fds[PIPE_READ], &msg, sizeof(Message));

        switch(msg.id) {
            case FROG_ID:
                for(i = old_frog_y; i - old_frog_y < FROG_Y_DIM; i++) {
                    mvwaprintw(main_scr, i, old_frog_x, WHITE_BLACK, "%*c", FROG_X_DIM, ' ');
                }
                print_frog(main_scr, msg.y, msg.x, frog_restore_colors);
                old_frog_y = msg.y;
                old_frog_x = msg.x;
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
                signal_all(process_pids, SIGKILL);
                return MANCHE_CLOSE;
        }
        wrefresh(main_scr);
    }
    return 0;
}