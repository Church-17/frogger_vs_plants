#include <unistd.h>
#include <sys/wait.h>
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
#define CLOSE_GAME 'q'
#define PAUSE_GAME 'p'
#define MANCHE_LOST -1
#define N_LIFES 3
#define N_HOLES 5

// Function prototypes
int play(void);
int play_manche(bool* holes_occupied, int n_lifes);

// Start demo in main_scr
void menu_bg(void) {
    wctrprintw(main_scr, 1, TITLE);
    box(main_scr, 0, 0);
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
    return do_quit;
}

// Play a game handling more manche
int play(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    int i;
    int manche_scores[N_HOLES] = {0};   // array with the scores for each manche
    int score = 0;
    int n_lifes = N_LIFES;

    for(int index_manche = 0; index_manche < N_HOLES; index_manche++) {
        manche_scores[index_manche] = play_manche(holes_occupied, n_lifes);
        if(manche_scores[index_manche] == MANCHE_LOST) {
            if(n_lifes > 0) {
                n_lifes--;
                index_manche--;
            } else {
                manche_scores[index_manche] = 0;
                break;
            }
        }
    }
    for(i = 0; i < N_HOLES; i++) {
        score += manche_scores[i];
    }
    score *= n_lifes+1;
    return gameover_menu(score);
}

// Play a manche
int play_manche(bool* holes_occupied, int n_lifes) {
    int i;
    pid_t array_pids[30];   // pids for every process
    List_pid process_pids;
    process_pids.list = array_pids;
    process_pids.len = 0;
    
    // Pipe
    int pipe_fds[PIPE_DIM];
    piper(pipe_fds); // Starts the pipe handling the errors

    process_pids.list[FROG_ID] = forker(&process_pids); // Calls the fork handling the errors
    (process_pids.len)++;
    if(process_pids.list[FROG_ID] == PID_CHILD) {
        // frog();
        _exit(ERR_FORK); // Handle unexpected process termination
    }
    
    // Store old coordinates
    int old_frog_y = MAIN_ROWS/2, old_frog_x = MAIN_COLS/2;

    // Colors under frog to restore
    int frog_restore_colors[FROG_Y_DIM] = {COLOR_BLACK, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK};

    Message msg;

    close(pipe_fds[PIPE_WRITE]);

    while(TRUE) {
        // read(pipe_fds[PIPE_READ], &msg, sizeof(Message));
        msg.id = 0;
        msg.x = 10;
        msg.y = 10;

        switch(msg.id) {
            case FROG_ID:
                for(i = old_frog_y; i - old_frog_y < FROG_Y_DIM; i++) {
                    mvwaprintw(main_scr, i, old_frog_x, WHITE_BLACK, "%*c", FROG_X_DIM, ' ');
                }
                print_frog(main_scr, msg.y, msg.x, frog_restore_colors);
                break;
        }
        wgetch(main_scr);
    }
    return 0;
}