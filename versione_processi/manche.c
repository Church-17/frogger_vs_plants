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
#define RESIZE_TIME_THRESHOLD 100
#define LIM_N_PROCESS 100
#define LIM_N_CROCCODILE (MIN_BULLET_ID - MIN_CROCCODILE_ID)

// Play a manche, return game vars with in gamevar.timer the time remaining or a manche_id
Game_t play_manche(bool* holes_occupied, int n_lifes) {
    // Erase old game or demo
    wclear(main_scr);
    wrefresh(main_scr);

    // Init vars
    int i;
    pid_t array_pids[LIM_N_PROCESS] = {0}; // Pids for every process
    List_pid process_pids;
    process_pids.list = array_pids;
    process_pids.len = LIM_N_PROCESS;
    int croccodile_params[3];
    int stream_speed[N_WATER_STREAM];
    int stream_last[N_WATER_STREAM]; // Track which croccodile was the last of each stream

    // Pipe
    int pipe_fds[PIPE_DIM];
    piper(pipe_fds); // Starts the pipe handling the errors

    // Forks
    forker(pipe_fds, &process_pids, FROG_ID, frog_process, NULL); // Calls the fork for frog process handling the errors
    forker(pipe_fds, &process_pids, TIME_ID, time_process, NULL); // Calls the fork for time process handling the errors
    for(i = 0; i < N_WATER_STREAM; i++) {
        do { // Randomize speed of each stream
            stream_speed[i] = rand_range(-3, 3) * 100;
        } while (stream_speed[i] == 0); // Speed must not be 0
        // Write croccodile params
        croccodile_params[0] = i + MIN_CROCCODILE_ID;
        croccodile_params[1] = stream_last[i] = i;
        croccodile_params[2] = stream_speed[i];
        // Calls the fork for croccodile process handling the errors
        forker(pipe_fds, &process_pids, croccodile_params[0], croccodile_process, croccodile_params);
    }
    
    // --- PARENT PROCESS ---

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int frog_on_croccodile = FROG_NOT_ON_CROCCODILE, croccodile_stream, croccodile_id;
    time_t resize_time = 0; // Var to store time of the last continue to prevent multiple resize message at once
    attr_t restore_color; // Variable for save color to restore
    Message msg; // Define msg to store pipe message

    // Init game vars
    Game_t gamevar;
    gamevar.frog.y = INIT_FROG_Y;
    gamevar.frog.x = INIT_FROG_X;
    gamevar.timer = TIME_MANCHE;
    alloc(Position, gamevar.croccodiles.list, LIM_N_CROCCODILE);
    for(i = 0; i < N_WATER_STREAM; i++) {
        gamevar.croccodiles.list[i].y = INCOMING_CROCCODILE;
    }
    for(i = N_WATER_STREAM; i < LIM_N_CROCCODILE; i++) {
        gamevar.croccodiles.list[i].y = FREE_CROCCODILE;
    }
    gamevar.croccodiles.len = LIM_N_CROCCODILE;
    gamevar.lifes = n_lifes;
    gamevar.holes_occupied = holes_occupied;
    gamevar.stream_speed = stream_speed;

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
                // Restore colors of old frog position
                if(gamevar.frog.y < LINE_RIVER) {
                    restore_color = BANK_BG; // If frog was on bank 1 set purple
                } else if(gamevar.frog.y < LINE_BANK_2) {
                    restore_color = GOOD_CROCCODILE_BG; // If frog was on croccodile set dark green
                } else {
                    restore_color = BANK_BG; // If frog was on bank 2 set purple
                }
                // De-print old frog with correct background
                for(i = 0; i < FROG_DIM_Y; i++) {
                    mvwaprintw(main_scr, i + gamevar.frog.y, gamevar.frog.x, restore_color, "%*s", FROG_DIM_X, "");
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

                // If the frog is in bank 1 and it isn't in front of a trap, it can't go ahead
                if(gamevar.frog.y < LINE_BANK_1) {
                    for(i = 0; i < N_HOLES; i++) {
                        if(gamevar.frog.x >= i*MAIN_COLS/N_HOLES + HOLE_PAD_X && gamevar.frog.x <= i*MAIN_COLS/N_HOLES + HOLE_PAD_X + HOLE_DIM_X - FROG_DIM_X && gamevar.holes_occupied[i] == FALSE) {
                            manche_ended = TRUE;
                            gamevar.holes_occupied[i] = TRUE;
                            break;
                        }
                    }
                    if(!manche_ended) {
                        gamevar.frog.y = LINE_BANK_1;
                    }
                }

                // Collision with river
                frog_on_croccodile = FROG_NOT_ON_CROCCODILE; // Reset croccodile index with frog on it
                if(gamevar.frog.y >= LINE_RIVER && gamevar.frog.y < LINE_BANK_2) { // If frog is in river zone...
                    for(i = 0; i < LIM_N_CROCCODILE; i++) {
                        if(gamevar.frog.y == gamevar.croccodiles.list[i].y && gamevar.frog.x >= gamevar.croccodiles.list[i].x && gamevar.frog.x <= gamevar.croccodiles.list[i].x + CROCCODILE_DIM_X - FROG_DIM_X) {
                            frog_on_croccodile = i;
                            break;
                        }
                    }
                    if(frog_on_croccodile == FROG_NOT_ON_CROCCODILE) {
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                    }
                }

                // Print frog if not dead
                if(gamevar.timer) {
                    print_frog(&gamevar);
                }
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

            // CROCCODILE
            if(msg.id >= MIN_CROCCODILE_ID && msg.id < MIN_BULLET_ID) {
                croccodile_id = msg.id - MIN_CROCCODILE_ID;
                croccodile_stream = (msg.y - LINE_RIVER) / FROG_DIM_Y;

                // Check if frog is on top
                if(frog_on_croccodile == croccodile_id) {
                    // De-print frog
                    restore_color = GOOD_CROCCODILE_BG;
                    for(i = gamevar.frog.y; i - gamevar.frog.y < FROG_DIM_Y; i++) {
                        mvwaprintw(main_scr, i, gamevar.frog.x, restore_color, "%*s", FROG_DIM_X, "");
                    }

                    // Update frog X position
                    if(stream_speed[croccodile_stream] > 0) {
                        gamevar.frog.x += MOVE_CROCCODILE_X;
                        if(gamevar.frog.x > LIM_RIGHT) { // If frog is outside limit...
                            gamevar.frog.x = LIM_RIGHT; // Move to limit
                            if(gamevar.frog.x < msg.x) {
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    } else {
                        gamevar.frog.x -= MOVE_CROCCODILE_X;
                        if(gamevar.frog.x < LIM_LEFT) { // If frog is outside limit...
                            gamevar.frog.x = LIM_LEFT; // Move to limit
                            if(gamevar.frog.x > msg.x + CROCCODILE_DIM_X - FROG_DIM_X) {
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    }
                }

                // De-print croccodile
                if(gamevar.croccodiles.list[croccodile_id].y > FREE_CROCCODILE) {
                    if(gamevar.croccodiles.list[croccodile_id].x < 0) {
                        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                            mvwaprintw(main_scr, i + gamevar.croccodiles.list[croccodile_id].y, 0, RIVER_BG, "%*s", CROCCODILE_DIM_X + gamevar.croccodiles.list[croccodile_id].x, "");
                        }
                    } else if(gamevar.croccodiles.list[croccodile_id].x < MAIN_COLS - CROCCODILE_DIM_X) {
                        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                            mvwaprintw(main_scr, i + gamevar.croccodiles.list[croccodile_id].y, gamevar.croccodiles.list[croccodile_id].x, RIVER_BG, "%*s", CROCCODILE_DIM_X, "");
                        }
                    } else {
                        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                            mvwaprintw(main_scr, i + gamevar.croccodiles.list[croccodile_id].y, gamevar.croccodiles.list[croccodile_id].x, RIVER_BG, "%*s", MAIN_COLS - gamevar.croccodiles.list[croccodile_id].x, "");
                        }
                    }
                }

                // Update saved coordinates
                gamevar.croccodiles.list[croccodile_id].x = msg.x;
                gamevar.croccodiles.list[croccodile_id].y = msg.y;

                // Print croccodile
                print_croccodile(gamevar.croccodiles.list[croccodile_id]);

                // Print frog is it's on croccodile
                if(frog_on_croccodile == croccodile_id) {
                    print_frog(&gamevar);
                }

                // Free croccodile
                if(msg.x <= -CROCCODILE_DIM_X || msg.x >= MAIN_COLS) {
                    gamevar.croccodiles.list[croccodile_id].y = FREE_CROCCODILE;
                }

                // Check if needs to spawn another croccodile
                if(stream_last[croccodile_stream] == croccodile_id && ((stream_speed[croccodile_stream] > 0 && msg.x > 0) || (stream_speed[croccodile_stream] < 0 && msg.x < MAIN_COLS - CROCCODILE_DIM_X))) {
                    for(i = 0; i < LIM_N_CROCCODILE; i++) {
                        if(gamevar.croccodiles.list[i].y == FREE_CROCCODILE) break;
                    }
                    gamevar.croccodiles.list[i].y = INCOMING_CROCCODILE;
                    croccodile_params[0] = i + MIN_CROCCODILE_ID;
                    croccodile_params[1] = croccodile_stream;
                    croccodile_params[2] = stream_speed[croccodile_stream];
                    forker(pipe_fds, &process_pids, croccodile_params[0], croccodile_process, croccodile_params); // Calls the fork for time process handling the errors
                    stream_last[croccodile_stream] = i;
                }

                break;
            }

            // BULLET
            else {

            }

            break;
        }
        wrefresh(main_scr);
    }
    signal_all(process_pids, SIGKILL); // Killing all child processes
    free(gamevar.croccodiles.list); // Free allocated memory
    return gamevar;
}
