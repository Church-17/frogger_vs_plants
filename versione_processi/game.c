#include <unistd.h>
#include <sys/wait.h>
#include "../main.h"
#include "../menu.h"
#include "../game.h"
#include "../str.h"
#include "../struct.h"
#include "../utils.h"
#include "process.h"

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

int play(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    int i;
    int manche_scores[N_HOLES] = {0};
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

int play_manche(bool* holes_occupied, int n_lifes) {
    int i;
    Message buff;
    pid_t array_pids[30];
    List_pid process_pids;
    process_pids.list = array_pids;
    process_pids.len = 0;

    int pipe_fds[PIPE_DIM];
    piper(pipe_fds);

    forker(&process_pids);
    if(process_pids.list[0] == 0) {
        frog();
        _exit(ERR_FORK); // Handle SUS process termination
    }
    
    close(pipe_fds[PIPE_WRITE]);

    while(TRUE) {
        read(pipe_fds[PIPE_READ], &buff, sizeof(Message));
        switch(buff.id) {
            case 0:
                
        }
    }
    return 0;
}