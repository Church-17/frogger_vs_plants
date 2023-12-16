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

// Play a manche, return game vars with in gamevar.timer the time remaining or a manche_id
Game_t play_manche(int score, int n_lifes, bool* holes_occupied) {
    // Init vars
    int i, j;
    pid_t array_pids[LIM_N_ENTITIES] = {0}; // Pids for every process
    List_pid process_pids;
    process_pids.list = array_pids;
    process_pids.len = LIM_N_ENTITIES;
    int croccodile_params[N_CROCCODILE_PARAMS];
    int stream_speed[N_WATER_STREAM];

    // Pipe
    int pipe_fds[PIPE_DIM];
    piper(pipe_fds); // Starts the pipe handling the errors

    // Forks
    forker(pipe_fds, &process_pids, FROG_ID, &frog_process, NULL); // Calls the fork for frog process handling the errors
    forker(pipe_fds, &process_pids, TIME_ID, &time_process, NULL); // Calls the fork for time process handling the errors
    for(i = 0; i < N_WATER_STREAM; i++) {
        // Randomize speed & direction of each stream
        stream_speed[i] = rand_range(2, 7) * (rand_range(0, 2) ? 1 : -1);
        // Write croccodile params
        croccodile_params[CROCCODILE_ID_INDEX] = i*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
        croccodile_params[CROCCODILE_STREAM_INDEX] = i;
        croccodile_params[CROCCODILE_SPEED_INDEX] = stream_speed[i];
        // Calls the fork for croccodile process handling the errors
        forker(pipe_fds, &process_pids, croccodile_params[CROCCODILE_ID_INDEX], &croccodile_process, croccodile_params);
    }
    
    // --- PARENT PROCESS ---

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int croccodile_stream, croccodile_id, next_croccodile_id; // Helper vars for croccodile
    int stream_last[N_WATER_STREAM] = {0}; // Track which croccodile was the last of each stream
    time_t resize_time = 0; // Var to store time of the last continue to prevent multiple resize message at once
    attr_t restore_color; // Variable for save color to restore
    Message msg; // Define msg to store pipe message

    // Init game vars
    Game_t gamevar;
    gamevar.holes_occupied = holes_occupied;
    gamevar.timer = TIME_MANCHE;
    gamevar.score = score;
    gamevar.lifes = n_lifes;
    gamevar.frog_on_croccodile = FROG_NOT_ON_CROCCODILE;
    gamevar.stream_speed = stream_speed;
    gamevar.frog.y = INIT_FROG_Y;
    gamevar.frog.x = INIT_FROG_X;
    alloc(Position*, gamevar.croccodiles, N_WATER_STREAM);
    for(i = 0; i < N_WATER_STREAM; i++) { // Croccodile array per stream
        alloc(Position, gamevar.croccodiles[i], MAX_CROCCODILE_PER_STREAM);
        gamevar.croccodiles[i][0].y = INCOMING_CROCCODILE; // Mark as incoming the first croccodile of each stream
        for(j = 1; j < MAX_CROCCODILE_PER_STREAM; j++) {
            gamevar.croccodiles[i][j].y = FREE_CROCCODILE; // Mark as free the other croccodiles of each stream
        }
    }

    // Erase old game or demo & print all elements of the game
    wclear(main_scr);
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
                gamevar.frog_on_croccodile = FROG_NOT_ON_CROCCODILE; // Reset croccodile index with frog on it
                if(gamevar.frog.y >= LINE_RIVER && gamevar.frog.y < LINE_BANK_2) { // If frog is in river zone...
                    croccodile_stream = (gamevar.frog.y - LINE_RIVER) / FROG_DIM_Y;
                    for(i = 0; i < MAX_CROCCODILE_PER_STREAM; i++) { // Check if frog is on a existing croccodile
                        if(gamevar.croccodiles[croccodile_stream][i].y >= 0 && gamevar.frog.x >= gamevar.croccodiles[croccodile_stream][i].x && gamevar.frog.x <= gamevar.croccodiles[croccodile_stream][i].x + CROCCODILE_DIM_X - FROG_DIM_X) {
                            gamevar.frog_on_croccodile = i + croccodile_stream*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
                            break;
                        }
                    }
                    if(gamevar.frog_on_croccodile == FROG_NOT_ON_CROCCODILE) { // If frog isn't on a croccodile: manche lost
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                    }
                }

                print_frog(&gamevar);
                break;

            // TIMER
            case TIME_ID:
                gamevar.timer = msg.y;
                if(gamevar.timer <= 0) {
                    manche_ended = TRUE;
                }
                print_time(gamevar.timer);
                break;

            // RESIZE AND AUTO-PAUSE
            case RESIZE_ID:
                // Check the current time with the last continue to prevent multiple resize message at once
                if(timestamp() - resize_time < RESIZE_TIME_THRESHOLD) {
                    break;
                }
                // Put game in pause (resize_proc will be called by menu)
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
                croccodile_stream = (msg.y - LINE_RIVER) / FROG_DIM_Y;
                croccodile_id = msg.id - MIN_CROCCODILE_ID - croccodile_stream*MAX_CROCCODILE_PER_STREAM;

                // Check if frog is on top
                if(gamevar.frog_on_croccodile == msg.id) {
                    // De-print frog
                    restore_color = GOOD_CROCCODILE_BG;
                    for(i = gamevar.frog.y; i - gamevar.frog.y < FROG_DIM_Y; i++) {
                        mvwaprintw(main_scr, i, gamevar.frog.x, restore_color, "%*s", FROG_DIM_X, "");
                    }

                    // Update frog X position
                    if(stream_speed[croccodile_stream] > 0) {
                        gamevar.frog.x += MOVE_CROCCODILE_X; // Move frog with croccodile
                        if(gamevar.frog.x > LIM_RIGHT) { // If frog is outside limit...
                            gamevar.frog.x = LIM_RIGHT; // Move to limit
                            if(gamevar.frog.x < msg.x) { // If now frog was outside croccodile: manche lost
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    } else {
                        gamevar.frog.x -= MOVE_CROCCODILE_X;
                        if(gamevar.frog.x < LIM_LEFT) { // If frog is outside limit...
                            gamevar.frog.x = LIM_LEFT; // Move to limit
                            if(gamevar.frog.x > msg.x + CROCCODILE_DIM_X - FROG_DIM_X) { // If now frog was outside croccodile: manche lost
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    }
                }

                // De-print croccodile
                if(gamevar.croccodiles[croccodile_stream][croccodile_id].y >= 0) {
                    if(gamevar.croccodiles[croccodile_stream][croccodile_id].x < 0) {
                        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                            mvwaprintw(main_scr, i + gamevar.croccodiles[croccodile_stream][croccodile_id].y, 0, RIVER_BG, "%*s", CROCCODILE_DIM_X + gamevar.croccodiles[croccodile_stream][croccodile_id].x, "");
                        }
                    } else if(gamevar.croccodiles[croccodile_stream][croccodile_id].x < MAIN_COLS - CROCCODILE_DIM_X) {
                        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                            mvwaprintw(main_scr, i + gamevar.croccodiles[croccodile_stream][croccodile_id].y, gamevar.croccodiles[croccodile_stream][croccodile_id].x, RIVER_BG, "%*s", CROCCODILE_DIM_X, "");
                        }
                    } else {
                        for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                            mvwaprintw(main_scr, i + gamevar.croccodiles[croccodile_stream][croccodile_id].y, gamevar.croccodiles[croccodile_stream][croccodile_id].x, RIVER_BG, "%*s", MAIN_COLS - gamevar.croccodiles[croccodile_stream][croccodile_id].x, "");
                        }
                    }
                }

                // Update saved coordinates
                gamevar.croccodiles[croccodile_stream][croccodile_id].x = msg.x;
                gamevar.croccodiles[croccodile_stream][croccodile_id].y = msg.y;

                // Print croccodile
                print_croccodile(gamevar.croccodiles[croccodile_stream][croccodile_id], stream_speed > 0);

                // Print frog is it's on croccodile
                if(gamevar.frog_on_croccodile == msg.id) {
                    print_frog(&gamevar);
                }

                // Free croccodile
                if(msg.x <= -CROCCODILE_DIM_X || msg.x >= MAIN_COLS) { // If croccodile was out of screen...
                    gamevar.croccodiles[croccodile_stream][croccodile_id].y = FREE_CROCCODILE; // mark it as free
                }

                // Check if needs to spawn another croccodile (if last croccodile is completely in screen)
                if(stream_last[croccodile_stream] == croccodile_id && (stream_speed[croccodile_stream] > 0 ? (msg.x > 0) : (msg.x < MAIN_COLS - CROCCODILE_DIM_X))) {
                    next_croccodile_id = mod(croccodile_id+1, MAX_CROCCODILE_PER_STREAM);
                    if(gamevar.croccodiles[croccodile_stream][next_croccodile_id].y == FREE_CROCCODILE) { // Check if next croccodile in same stream is free
                        gamevar.croccodiles[croccodile_stream][next_croccodile_id].y = INCOMING_CROCCODILE; // Mark the founded free croccodile as incoming
                        croccodile_params[CROCCODILE_ID_INDEX] = next_croccodile_id + croccodile_stream*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
                        croccodile_params[CROCCODILE_STREAM_INDEX] = croccodile_stream;
                        croccodile_params[CROCCODILE_SPEED_INDEX] = stream_speed[croccodile_stream];
                        forker(pipe_fds, &process_pids, croccodile_params[CROCCODILE_ID_INDEX], &croccodile_process, croccodile_params); // Calls the fork for croccodile process handling the errors
                        stream_last[croccodile_stream] = next_croccodile_id; // Update last croccodile of stream
                    }
                }

                break;
            }

            // BULLET
            else {
                
                break;
            }

            break;
        }
        wrefresh(main_scr);
    }
    signal_all(process_pids, SIGKILL); // Killing all child processes
    // Free allocated memory
    for(i = 0; i < N_WATER_STREAM; i++) {
        free(gamevar.croccodiles[i]);
    }
    free(gamevar.croccodiles);
    return gamevar;
}
