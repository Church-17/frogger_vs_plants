#include "../main.h"
#include "../menu.h"
#include "../game.h"
#include "../sprites.h"
#include "../str.h"
#include "../struct.h"
#include "../utils.h"
#include "process.h"
#include "frog.h"

// Define constant
#define LIM_N_PROCESS 30
#define MANCHE_LOST 0
#define N_LIFES 3
#define N_HOLES 5

// Function prototypes
int play(void);
int play_manche(bool* holes_occupied, int n_lifes);

// Start demo in main_scr
void menu_bg(void) {
    wctrprintw(main_scr, 1, TITLE);
}

// Manage more games
bool game() {
    // Init vars
    bool do_play = TRUE, do_quit = FALSE; // Flag
    int game_ret;

    while(do_play) {

        game_ret = play();

        switch(game_ret) {
            case OVER_RETR_ID: // Play
                break;

            case OVER_BACK_ID: // Back to menu
                do_play = FALSE;
                break;

            case OVER_QUIT_ID:
                do_play = FALSE;
                do_quit = TRUE;
                break;

            default:
                break;
        }
    }
    wclear(main_scr);
    wrefresh(main_scr);
    return do_quit;
}

// Play a game handling more manche
int play(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    int i, score = 0, n_lifes = N_LIFES;
    int manche_remained_time[N_HOLES] = {0}; // Array with the remained time of each manche

    // Loop for play n manche saving the remained time and updating lifes
    for(i = 0; i < N_HOLES && n_lifes; i++) {
        manche_remained_time[i] = play_manche(holes_occupied, n_lifes);
        switch(manche_remained_time[i]) {
            case MANCHE_LOST:
                n_lifes--;
                if(n_lifes > 0) {
                    i--;
                }
                break;

            case CLOSE_ID:
                return gameover_menu(0);
                break;

            default: break;
        }
    }

    for(i = 0; i < N_HOLES; i++) {
        score += manche_remained_time[i];
    }
    score *= n_lifes;
    return gameover_menu(score);
}

// Play a manche
int play_manche(bool* holes_occupied, int n_lifes) {
    wclear(main_scr); // Erase old game or menu_bg
    wrefresh(main_scr);

    int i;
    pid_t array_pids[30]; // Pids for every process
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

            case CLOSE_ID:
                signal_all(process_pids, SIGKILL);
                return CLOSE_ID;
        }
        wrefresh(main_scr);
    }
    return 0;
}