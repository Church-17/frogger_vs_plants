// Include libs
#include "../manche.h"
#include "../menu.h"
#include "../res.h"
#include "../utils.h"
#include "../struct.h"
#include "process.h"
#include "frog.h"
#include "time.h"
#include "croccodile.h"

// Define constant
#define LIM_N_PROCESS 30
#define RESIZE_TIME_THRESHOLD 100

// Play a manche, return game vars with in gamevar.timer the time remaining or a manche_id
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
    int croccodile_params[3];
    int stream_speed[N_WATER_STREAM] = {0};
    int stream_last[N_WATER_STREAM];

    for (i = 0; i < N_WATER_STREAM; i++) {
        while (stream_speed[i] == 0) {
            stream_speed[i] = rand_range(-10, 11) * 100;
        }        
        
    }

    for (i = 0; i < N_WATER_STREAM; i++) {
        stream_last[i] = i + MIN_CROCCODILE_ID;       
    }
    
    // Pipe
    int pipe_fds[PIPE_DIM];
    piper(pipe_fds); // Starts the pipe handling the errors

    // Fork
    forker(pipe_fds, &process_pids, FROG_ID, frog_process, NULL); // Calls the fork for frog process handling the errors
    forker(pipe_fds, &process_pids, TIME_ID, time_process, NULL); // Calls the fork for frog process handling the errors

    for (i = 0; i < N_WATER_STREAM; i++) {
        croccodile_params[0] = i + MIN_CROCCODILE_ID;  
        croccodile_params[1] = i;  
        croccodile_params[2] = stream_speed[i];
        forker(pipe_fds, &process_pids, i + MIN_CROCCODILE_ID, croccodile_process, croccodile_params); // Calls the fork for time process handling the errors
    }
    
    // --- PARENT PROCESS ---

    close(pipe_fds[PIPE_WRITE]); // Close unused fd

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int frog_on_croccodile = -1;
    time_t resize_time = 0; // Var to store time of the last continue to prevent multiple resize message at once
    attr_t restore_color; // Variable for save color to restore
    Message msg; // Define msg to store pipe message

    // Init game vars
    Game_t gamevar;
    gamevar.frog.y = INIT_FROG_Y;
    gamevar.frog.x = INIT_FROG_X;
    gamevar.timer = TIME_MANCHE;
    alloc(Position, gamevar.croccodiles, 10);
    gamevar.lifes = n_lifes;
    gamevar.holes_occupied = holes_occupied;

    // Print all elements of the game
    print_game(&gamevar);
    wrefresh(main_scr);

    // Manche loop
    while(!manche_ended) {
        
        // Read from pipe
        reader(pipe_fds[PIPE_READ], &msg);

        switch(msg.id) {
            // FROG
            case FROG_ID:
                // Restore old frog position
                if(gamevar.frog.y < LINE_RIVER) {restore_color = GREEN_PURPLE;}
                else if(gamevar.frog.y < LINE_BANK_2) {
                    if (frog_on_croccodile != -1) {
                        restore_color = MAGENTA_GREEN;
                    } else {
                        restore_color = GREEN_BLUE;
                    }
                }
                else {restore_color = GREEN_PURPLE;}
                for(i = gamevar.frog.y; i - gamevar.frog.y < FROG_DIM_Y; i++) {
                    mvwaprintw(main_scr, i, gamevar.frog.x, restore_color, "%*s", FROG_DIM_X, "");
                }

                // Update frog Y position
                if(gamevar.frog.y >= LIM_UP && gamevar.frog.y <= LIM_DOWN) { // If frog can move...
                    gamevar.frog.y += msg.y; // Update coordinate
                    if(gamevar.frog.y < LIM_UP) { // If frog is outside limit...q
                        gamevar.frog.y = LIM_UP; // Move to limit
                    } else if(gamevar.frog.y > LIM_DOWN) { // If frog is outside limit...
                        gamevar.frog.y = LIM_DOWN; // Move to limit
                    }
                }
                // Update frog X position
                if(gamevar.frog.x >= LIM_LEFT && gamevar.frog.x <= LIM_RIGHT) { // If frog can move...
                    gamevar.frog.x += msg.x; // Update coordinate
                    if(gamevar.frog.x < LIM_LEFT) { // If frog is outside limit...
                        gamevar.frog.x = LIM_LEFT; // Move to limit
                    } else if(gamevar.frog.x > LIM_RIGHT) { // If frog is outside limit...
                        gamevar.frog.x = LIM_RIGHT; // Move to limit
                    }
                }

                // If the frog isn't in front of a trap, it can't go ahead
                if(gamevar.frog.y <= LINE_RIVER && !in_hole_line(gamevar.frog)) {
                    if(gamevar.frog.y < LINE_BANK_1) { 
                        gamevar.frog.y = LINE_BANK_1;
                    }
                }

                frog_on_croccodile = -1;
                if (gamevar.frog.y >= LINE_RIVER && gamevar.frog.y < LINE_BANK_2) {
                    for (i = 0; i < 10; i++) {
                        if (gamevar.frog.y == gamevar.croccodiles[i].y && gamevar.frog.x >= gamevar.croccodiles[i].x && gamevar.frog.x <= gamevar.croccodiles[i].x + CROCCODILE_DIM_X - FROG_DIM_X) {
                            frog_on_croccodile = i;
                            break;
                        }
                    }

                    if (frog_on_croccodile == -1) {
                        manche_ended = TRUE;
                        gamevar.timer = MANCHE_LOST;
                    }
                }

                print_frog(&gamevar);
                break;

            // TIMER
            case TIME_ID:
                if(gamevar.timer <= 0) {
                    manche_ended = TRUE;
                } else {
                    gamevar.timer--;
                }
                print_time(gamevar.timer);
                break;

            // RESIZE AND AUTO-PAUSE
            case RESIZE_ID:
                // Check the current time with the last continue to prevent multiple resize message at once
                if(timestamp() - resize_time < RESIZE_TIME_THRESHOLD) {
                    break;
                }
                // Call resize procedure
                resize_proc(NULL, 0, 0, &gamevar);
                // Put game in pause
            // PAUSE
            case PAUSE_ID:
                signal_all(process_pids, SIGSTOP); // Pausing all child processes
                i = pause_menu(&gamevar);
                switch (i) {
                    case PAUSE_RES_ID:
                        break;
                    
                    case PAUSE_RETR_ID:
                        gamevar.timer = MANCHE_RETR;
                        manche_ended = TRUE;
                        break;

                    case PAUSE_BACK_ID:
                        gamevar.timer = MANCHE_CLOSE;
                        manche_ended = TRUE;
                        break;

                    case PAUSE_QUIT_ID:
                        gamevar.timer = MANCHE_QUIT;
                        manche_ended = TRUE;
                        break;
                }
                // If continue:
                print_game(&gamevar); // Redraw game
                signal_all(process_pids, SIGCONT); // Resume all child processes
                resize_time = timestamp(); // Save the current time to prevent multiple resize message at once
                break;

            // CLOSE
            case CLOSE_ID:
                signal_all(process_pids, SIGSTOP); // Pausing all child processes
                i = quit_menu(&gamevar);    
                switch (i) {
                    case NO_ID:
                        break;
                    
                    case YES_ID:
                        gamevar.timer = MANCHE_CLOSE;
                        manche_ended = TRUE;
                        break;
                }
                // If continue:
                print_game(&gamevar); // Redraw game
                signal_all(process_pids, SIGCONT); // Resume all child processes
                resize_time = timestamp(); // Save the current time to prevent multiple resize message at once
                break;

            default:
                if (msg.id >= MIN_CROCCODILE_ID && msg.id < 100) {
                    int n_stream = msg.y*FROG_DIM_X + LINE_BANK_1;

                    if (stream_speed[n_stream] > 0) { // if the stream has positive speed

                        for (i = msg.y; i < msg.y + CROCCODILE_DIM_Y; i++) {
                            if (msg.x <= MAIN_COLS - CROCCODILE_DIM_X) {
                                mvwaprintw(main_scr, i, msg.x > 0 ? msg.x : 0, MAGENTA_GREEN, "%*s", CROCCODILE_DIM_X + (msg.x < 0 ? msg.x : 0), " ");
                            } else {
                                mvwaprintw(main_scr, i, msg.x, MAGENTA_GREEN, "%*s", MAIN_COLS - msg.x, " ");
                            }

                        }

                        if (msg.x > 0) {
                            for (i = msg.y; i < msg.y + CROCCODILE_DIM_Y; i++) {
                                mvwaprintw(main_scr, i, msg.x - MOVE_CROCCODILE_X, GREEN_BLUE, "%*s", MOVE_CROCCODILE_X, " ");
                            }
                        }
                    } else { // if the stream has negative speed

                        for (i = msg.y; i < msg.y + CROCCODILE_DIM_Y; i++) {
                            if (msg.x <= MAIN_COLS - CROCCODILE_DIM_X) {
                                mvwaprintw(main_scr, i, msg.x > 0 ? msg.x : 0, MAGENTA_GREEN, "%*s", CROCCODILE_DIM_X + (msg.x < 0 ? msg.x : 0), " ");
                            } else {
                                mvwaprintw(main_scr, i, msg.x, MAGENTA_GREEN, "%*s", MAIN_COLS - msg.x, " ");
                            }

                        }

                        if (msg.x <= MAIN_COLS - CROCCODILE_DIM_X) {
                            for (i = msg.y; i < msg.y + CROCCODILE_DIM_Y; i++) {
                                mvwaprintw(main_scr, i, msg.x + CROCCODILE_DIM_X, GREEN_BLUE, "%*s", MOVE_CROCCODILE_X, " ");
                            }
                        }
                    }

                    gamevar.croccodiles[msg.id - MIN_CROCCODILE_ID].x = msg.x;
                    gamevar.croccodiles[msg.id - MIN_CROCCODILE_ID].y = msg.y;

                    if (frog_on_croccodile == msg.id - MIN_CROCCODILE_ID) {

                        if(gamevar.frog.y < LINE_RIVER) {restore_color = GREEN_PURPLE;}
                        else if(gamevar.frog.y < LINE_BANK_2) {restore_color = MAGENTA_GREEN;}
                        else {restore_color = GREEN_PURPLE;}
                        for(i = gamevar.frog.y; i - gamevar.frog.y < FROG_DIM_Y; i++) {
                            mvwaprintw(main_scr, i, gamevar.frog.x, restore_color, "%*s", FROG_DIM_X, "");
                        }

                        // Update frog X position
                        if(gamevar.frog.x >= LIM_LEFT && gamevar.frog.x <= LIM_RIGHT) { // If frog can move...
                            gamevar.frog.x += MOVE_CROCCODILE_X; // Update coordinate
                            if(gamevar.frog.x < LIM_LEFT) { // If frog is outside limit...
                                gamevar.frog.x = LIM_LEFT; // Move to limit
                            } else if(gamevar.frog.x > LIM_RIGHT) { // If frog is outside limit...
                                gamevar.frog.x = LIM_RIGHT; // Move to limit
                            }
                        }

                        print_frog(&gamevar);
                    }
                
                    break;
                }
        }
        wrefresh(main_scr);
    }
    signal_all(process_pids, SIGKILL); // Killing all child processes
    return gamevar;
}
