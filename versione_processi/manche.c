// Include libs
#include <signal.h>
#include "../main.h"
#include "../menu.h"
#include "../manche.h"
#include "../utils.h"
#include "../music.h"
#include "../struct.h"
#include "process.h"
#include "entity.h"

// Define constant
#define RESIZE_TIME_THRESHOLD 100
#define REFRESH_TIME_THRESHOLD 100

// Play a manche, return game vars with in gamevar.timer the time remaining or a manche_id
Game_t play_manche(int score, int n_lifes, bool* holes_occupied) {
    // Init vars
    bool occupied_plant;
    int i, j;
    pid_t array_pids[LIM_N_ENTITIES] = {0}; // Pids for every process
    List_pid process_pids;
    process_pids.list = array_pids;
    process_pids.len = LIM_N_ENTITIES;
    int fork_params[N_CROCCODILE_PARAMS];
    int stream_speed[N_WATER_STREAM];
    int plants_x[N_PLANTS];

    // Pipe
    int pipe_fds[PIPE_DIM];
    piper(pipe_fds); // Starts the pipe handling the errors

    // Forks
    forker(pipe_fds, &process_pids, FROG_ID, &frog_process, NULL); // Calls the fork for frog process handling the errors
    forker(pipe_fds, &process_pids, TIME_ID, &time_process, NULL); // Calls the fork for time process handling the errors
    for(i = 0; i < N_WATER_STREAM; i++) {
        // Randomize speed & direction of each stream
        stream_speed[i] = rand_range(MIN_STREAM_SPEED, MAX_STREAM_SPEED) * (rand_range(0, 2) ? 1 : -1);
        // Write croccodile params
        fork_params[CROCCODILE_ID_INDEX] = i*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
        fork_params[CROCCODILE_STREAM_INDEX] = i;
        fork_params[CROCCODILE_SPEED_INDEX] = stream_speed[i];
        // Calls the fork for croccodile process handling the errors
        forker(pipe_fds, &process_pids, fork_params[CROCCODILE_ID_INDEX], &croccodile_process, fork_params);
    }
    for(i = 0; i < N_PLANTS; i++) {
        fork_params[PLANT_ID_INDEX] = MIN_PLANT_ID + i;
        do {
            occupied_plant = FALSE;
            plants_x[i] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
            for(j = 0; j < i; j++) {
                if(plants_x[i] > plants_x[j] - PLANT_DIM_X && plants_x[i] < plants_x[j] + PLANT_DIM_X) {
                    occupied_plant = TRUE;
                    break;
                }
            }
        } while(occupied_plant);
        fork_params[PLANT_X_INDEX] = plants_x[i];
        forker(pipe_fds, &process_pids, fork_params[PLANT_ID_INDEX], &plant_process, fork_params);
    }
    
    // --- PARENT PROCESS ---

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int croccodile_stream, entity_id, next_croccodile_id, restore_croccodile_x, restore_croccodile_len; // Helper vars for croccodile
    int stream_last[N_WATER_STREAM] = {0}; // Track which croccodile was the last of each stream
    int next_frog_bullet = 0, next_plant_bullet[N_PLANTS] = {0};
    time_t resize_time = 0, refresh_time = 0; // Var to store time of the last continue to prevent multiple resize message at once
    attr_t restore_color; // Variable for save color to restore
    Message msg; // Define msg to store pipe message

    // Init game vars
    Game_t gamevar;
    gamevar.holes_occupied = holes_occupied;
    gamevar.timer = TIME_MANCHE;
    gamevar.score = score;
    gamevar.lifes = n_lifes;
    gamevar.win = IN_GAME;
    gamevar.frog_on_croccodile = FROG_NOT_ON_CROCCODILE;
    gamevar.free_frog_bullet = MAX_BULLETS_PER_FROG;
    gamevar.stream_speed = stream_speed;

    gamevar.frog.y = FROG_INIT_Y;
    gamevar.frog.x = FROG_INIT_X;
    alloc(Position, gamevar.frog_bullets, MAX_BULLETS_PER_FROG);
    for(i = 0; i < MAX_BULLETS_PER_FROG; i++) {
        gamevar.frog_bullets[i].y = FREE_BULLET; // Mark as free each frog bullet
    }

    alloc(Position, gamevar.plants, N_PLANTS);
    alloc(Position*, gamevar.plants_bullets, N_PLANTS);
    for(i = 0; i < N_PLANTS; i++) {
        gamevar.plants[i].y = INCOMING_PLANT; // Mark as incoming each plant
        alloc(Position, gamevar.plants_bullets[i], MAX_BULLETS_PER_PLANT);
        for(j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
            gamevar.plants_bullets[i][j].y = FREE_BULLET; // Mark as free the bullets of each plant
        }
    }

    alloc(Position*, gamevar.croccodiles, N_WATER_STREAM);
    alloc(bool*, gamevar.bad_croccodiles, N_WATER_STREAM);
    for(i = 0; i < N_WATER_STREAM; i++) { // Croccodile array per stream
        alloc(Position, gamevar.croccodiles[i], MAX_CROCCODILE_PER_STREAM);
        alloc(bool, gamevar.bad_croccodiles[i], MAX_CROCCODILE_PER_STREAM);
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
                switch(i) {
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
                switch(i) {
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
            
            // TIMER
            case TIME_ID:
                gamevar.timer = msg.sig;
                if(gamevar.timer <= 0) {
                    manche_ended = TRUE;
                }
                print_time(gamevar.timer);
                break;

            
            // FROG
            case FROG_ID:
                if(msg.sig == FROG_SHOT_SIG) {
                    if(gamevar.frog_bullets[next_frog_bullet].y == FREE_BULLET) {
                        fork_params[BULLET_ID_INDEX] = next_frog_bullet + MIN_FROG_BULLET_ID;
                        fork_params[BULLET_Y_INDEX] = gamevar.frog.y - 1;
                        fork_params[BULLET_X_INDEX] = gamevar.frog.x + FROG_DIM_X/2;
                        forker(pipe_fds, &process_pids, fork_params[BULLET_ID_INDEX], &bullet_process, fork_params);
                        next_frog_bullet = mod(next_frog_bullet + 1, MAX_BULLETS_PER_FROG);
                        gamevar.free_frog_bullet--;
                        print_free_frog_bullet(gamevar.free_frog_bullet);
                    }
                    break;
                }

                // Restore colors of old frog position
                if(gamevar.frog.y < LINE_RIVER) {
                    restore_color = BANK_BG; // If frog was on bank 1 set purple
                } else if(gamevar.frog.y < LINE_BANK_2) {
                    if(gamevar.frog_on_croccodile >= 0) {
                        croccodile_stream = (gamevar.frog.y - LINE_RIVER) / FROG_DIM_Y;
                        entity_id = gamevar.frog_on_croccodile - MIN_CROCCODILE_ID - croccodile_stream*MAX_CROCCODILE_PER_STREAM;
                        if(gamevar.bad_croccodiles[croccodile_stream][entity_id]) {
                            restore_color = BAD_CROCCODILE_BG; // If frog was on bad croccodile set bordeaux
                        } else {
                            restore_color = GOOD_CROCCODILE_BG; // If frog was on good croccodile set dark green
                        }
                    }
                } else {
                    restore_color = BANK_BG; // If frog was on bank 2 set purple
                }
                // De-print old frog with correct background
                for(i = 0; i < FROG_DIM_Y; i++) {
                    mvwaprintw(main_scr, i + gamevar.frog.y, gamevar.frog.x, restore_color, "%*s", FROG_DIM_X, "");
                }

                // Update frog Y position
                if(gamevar.frog.y >= FROG_LIM_UP && gamevar.frog.y <= FROG_LIM_DOWN) { // If frog can move...
                    gamevar.frog.y += msg.y; // Update coordinate
                    if(gamevar.frog.y < FROG_LIM_UP) { // If frog is outside limit...q
                        gamevar.frog.y = FROG_LIM_UP; // Move to limit
                    } else if(gamevar.frog.y > FROG_LIM_DOWN) { // If frog is outside limit...
                        gamevar.frog.y = FROG_LIM_DOWN; // Move to limit
                    }
                }
                // Update frog X position
                if(gamevar.frog.x >= FROG_LIM_LEFT && gamevar.frog.x <= FROG_LIM_RIGHT) { // If frog can move...
                    gamevar.frog.x += msg.x; // Update coordinate
                    if(gamevar.frog.x < FROG_LIM_LEFT) { // If frog is outside limit...
                        gamevar.frog.x = FROG_LIM_LEFT; // Move to limit
                    } else if(gamevar.frog.x > FROG_LIM_RIGHT) { // If frog is outside limit...
                        gamevar.frog.x = FROG_LIM_RIGHT; // Move to limit
                    }
                }

                // If the frog is in bank 1 and it isn't in front of a trap, it can't go ahead
                if(gamevar.frog.y < LINE_BANK_1) {
                    for(i = 0; i < N_HOLES; i++) {
                        if(gamevar.frog.x >= i*MAIN_COLS/N_HOLES + (MAIN_COLS/N_HOLES - HOLE_DIM_X)/2 && gamevar.frog.x <= i*MAIN_COLS/N_HOLES + (MAIN_COLS/N_HOLES - HOLE_DIM_X)/2 + HOLE_DIM_X - FROG_DIM_X && gamevar.holes_occupied[i] == FALSE) {
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
                            kill(process_pids.list[gamevar.frog_on_croccodile], FROG_ON_CROCCODILE_SIG);
                            break;
                        }
                    }
                    if(gamevar.frog_on_croccodile == FROG_NOT_ON_CROCCODILE) { // If frog isn't on a croccodile: manche lost
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                    }
                }

                // Print frog
                print_frog(&gamevar);

                break;

            default:

            // CROCCODILE
            if(msg.id >= MIN_CROCCODILE_ID && msg.id < MIN_PLANT_ID) {
                croccodile_stream = ((msg.y) - LINE_RIVER) / FROG_DIM_Y;
                entity_id = msg.id - MIN_CROCCODILE_ID - croccodile_stream*MAX_CROCCODILE_PER_STREAM;
                gamevar.bad_croccodiles[croccodile_stream][entity_id] = (bool) msg.sig;

                // De-print croccodile
                if(gamevar.croccodiles[croccodile_stream][entity_id].y >= 0) { // If croccodile is not free or incoming...
                    if(gamevar.croccodiles[croccodile_stream][entity_id].x < 0) {
                        restore_croccodile_x = 0;
                        restore_croccodile_len = CROCCODILE_DIM_X + gamevar.croccodiles[croccodile_stream][entity_id].x;
                    } else if(gamevar.croccodiles[croccodile_stream][entity_id].x < MAIN_COLS - CROCCODILE_DIM_X) {
                        restore_croccodile_x = gamevar.croccodiles[croccodile_stream][entity_id].x;
                        restore_croccodile_len = CROCCODILE_DIM_X;
                    } else {
                        restore_croccodile_x = gamevar.croccodiles[croccodile_stream][entity_id].x;
                        restore_croccodile_len = MAIN_COLS - gamevar.croccodiles[croccodile_stream][entity_id].x;
                    }
                    for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                        mvwaprintw(main_scr, i + gamevar.croccodiles[croccodile_stream][entity_id].y, restore_croccodile_x, RIVER_BG, "%*s", restore_croccodile_len, "");
                    }
                }

                // Print croccodile or free it
                if(msg.x <= -CROCCODILE_DIM_X || msg.x >= MAIN_COLS || msg.sig == CROCCODILE_IMMERSION_SIG) { // If croccodile is out of screen...
                    gamevar.croccodiles[croccodile_stream][entity_id].y = FREE_CROCCODILE; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died croccodile process
                    process_pids.list[msg.id] = 0;
                } else {
                    gamevar.croccodiles[croccodile_stream][entity_id].x = msg.x;
                    gamevar.croccodiles[croccodile_stream][entity_id].y = msg.y;
                    print_croccodile(gamevar.croccodiles[croccodile_stream][entity_id], stream_speed > 0, msg.sig);
                }

                // Check if frog is on top
                if(gamevar.frog_on_croccodile == msg.id) {
                    if(msg.sig == CROCCODILE_IMMERSION_SIG) {
                        manche_ended = TRUE;
                        gamevar.timer = MANCHE_LOST;
                    } else if(stream_speed[croccodile_stream] > 0) { // Update frog X position
                        gamevar.frog.x += CROCCODILE_MOVE_X; // Move frog with croccodile
                        if(gamevar.frog.x > FROG_LIM_RIGHT) { // If frog is outside limit...
                            gamevar.frog.x = FROG_LIM_RIGHT; // Move to limit
                            if(gamevar.frog.x < msg.x) { // If now frog was outside croccodile: manche lost
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    } else {
                        gamevar.frog.x -= CROCCODILE_MOVE_X;
                        if(gamevar.frog.x < FROG_LIM_LEFT) { // If frog is outside limit...
                            gamevar.frog.x = FROG_LIM_LEFT; // Move to limit
                            if(gamevar.frog.x > msg.x + CROCCODILE_DIM_X - FROG_DIM_X) { // If now frog was outside croccodile: manche lost
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    }
                    
                    print_frog(&gamevar);
                }

                // Check if needs to spawn another croccodile (if last croccodile is completely in screen)
                if(stream_last[croccodile_stream] == entity_id && (stream_speed[croccodile_stream] > 0 ? (msg.x > 0) : (msg.x < MAIN_COLS - CROCCODILE_DIM_X))) {
                    next_croccodile_id = mod(entity_id+1, MAX_CROCCODILE_PER_STREAM);
                    if(gamevar.croccodiles[croccodile_stream][next_croccodile_id].y == FREE_CROCCODILE) { // Check if next croccodile in same stream is free
                        gamevar.croccodiles[croccodile_stream][next_croccodile_id].y = INCOMING_CROCCODILE; // Mark the founded free croccodile as incoming
                        fork_params[CROCCODILE_ID_INDEX] = next_croccodile_id + croccodile_stream*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
                        fork_params[CROCCODILE_STREAM_INDEX] = croccodile_stream;
                        fork_params[CROCCODILE_SPEED_INDEX] = stream_speed[croccodile_stream];
                        forker(pipe_fds, &process_pids, fork_params[CROCCODILE_ID_INDEX], &croccodile_process, fork_params); // Calls the fork for croccodile process handling the errors
                        stream_last[croccodile_stream] = next_croccodile_id; // Update last croccodile of stream
                    }
                }

                break;
            }

            // PLANT
            else if(msg.id >= MIN_PLANT_ID && msg.id < MIN_FROG_BULLET_ID) {
                entity_id = msg.id - MIN_PLANT_ID;
                if(msg.sig == PLANT_SPAWN_SIG) {
                    gamevar.plants[entity_id].y = msg.y;
                    gamevar.plants[entity_id].x = msg.x;
                    print_plant(gamevar.plants[entity_id]);
                }
                break;
            }

            // FROG BULLET
            else if(msg.id >= MIN_FROG_BULLET_ID && msg.id < MIN_PLANT_BULLET_ID) {
                entity_id = msg.id - MIN_FROG_BULLET_ID;

                // De-print
                if(gamevar.frog_bullets[entity_id].y >= 0) {
                    for(i = 0; i < BULLET_DIM_Y; i++) {
                        mvwaprintw(main_scr, gamevar.frog_bullets[entity_id].y, gamevar.frog_bullets[entity_id].x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                    }
                }

                // Print bullet or free it
                if(msg.y < LINE_BANK_1) { // If frog bullet is out of screen...
                    gamevar.free_frog_bullet++;
                    print_free_frog_bullet(gamevar.free_frog_bullet);
                    gamevar.frog_bullets[entity_id].y = FREE_BULLET; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                    process_pids.list[msg.id] = 0;
                } else {
                    gamevar.frog_bullets[entity_id].y = msg.y;
                    gamevar.frog_bullets[entity_id].x = msg.x;
                    print_bullet(gamevar.frog_bullets[entity_id]);
                }

                break;
            }

            break;
        }
        
        // Refresh
        if(timestamp() - refresh_time > REFRESH_TIME_THRESHOLD) {
            wrefresh(main_scr);
        }
    }
    signal_all(process_pids, SIGKILL); // Killing all child processes
    while(wait(NULL) > 0); // Wait all child processes

    // Free allocated memory
    for(i = 0; i < N_WATER_STREAM; i++) {
        free(gamevar.croccodiles[i]);
        free(gamevar.bad_croccodiles[i]);
    }
    for(i = 0; i < N_PLANTS; i++) {
        free(gamevar.plants_bullets[i]);
    }
    free(gamevar.croccodiles);
    free(gamevar.bad_croccodiles);
    free(gamevar.plants);
    free(gamevar.frog_bullets);
    free(gamevar.plants_bullets);
    gamevar.bad_croccodiles = NULL;
    gamevar.croccodiles = gamevar.plants_bullets = NULL;
    gamevar.plants = gamevar.frog_bullets = NULL;

    // Close file descriptors
    close(pipe_fds[PIPE_READ]);
    close(pipe_fds[PIPE_WRITE]);

    return gamevar;
}
