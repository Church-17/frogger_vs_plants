// Include libs
#include <unistd.h>
#include "../main.h"
#include "../menu.h"
#include "../manche.h"
#include "../music.h"
#include "../utils.h"
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
    List_pid process_pids = {array_pids, LIM_N_ENTITIES};
    int fork_params[N_ENTITY_PARAMS];
    int stream_speed[N_WATER_STREAM];
    int plants_x[N_PLANTS];

    // Pipe
    int pipe_fds[PIPE_DIM];
    if(pipe(pipe_fds) < 0) {
        quit(ERR_PIPE);
    }

    // Forks
    async_exec(pipe_fds, &process_pids, FROG_ID, &frog_process, NULL); // Calls the fork for frog process handling the errors
    async_exec(pipe_fds, &process_pids, TIME_ID, &time_process, NULL); // Calls the fork for time process handling the errors
    for(i = 0; i < N_WATER_STREAM; i++) {
        // Randomize speed & direction of each stream
        stream_speed[i] = rand_range(MIN_STREAM_SPEED, MAX_STREAM_SPEED) * (rand_range(0, 2) ? 1 : -1);
        // Write croccodile params
        fork_params[CROCCODILE_ID_INDEX] = i*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
        fork_params[CROCCODILE_STREAM_INDEX] = i;
        fork_params[CROCCODILE_SPEED_INDEX] = stream_speed[i];
        // Calls the fork for croccodile process handling the errors
        async_exec(pipe_fds, &process_pids, fork_params[CROCCODILE_ID_INDEX], &croccodile_process, fork_params);
    }
    for(i = 0; i < N_PLANTS; i++) {
        fork_params[PLANT_ID_INDEX] = MIN_PLANT_ID + i;
        do { // Randomize X coordinates of plant
            occupied_plant = FALSE;
            plants_x[i] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
            for(j = 0; j < i; j++) {
                // Check if any plants if anyready in that space
                if(plants_x[i] + PLANT_DIM_X > plants_x[j] && plants_x[i] < plants_x[j] + PLANT_DIM_X) {
                    occupied_plant = TRUE;
                    break;
                }
            }
        } while(occupied_plant);
        fork_params[PLANT_X_INDEX] = plants_x[i];
        async_exec(pipe_fds, &process_pids, fork_params[PLANT_ID_INDEX], &plant_process, fork_params);
    }
    
    // --- PARENT PROCESS ---

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int entity_stream, entity_id, next_croccodile_id, restore_croccodile_x, restore_croccodile_len; // Helper vars for croccodile
    int stream_last[N_WATER_STREAM] = {0}; // Track which croccodile was the last of each stream
    int next_frog_bullet = 0, next_plant_bullet[N_PLANTS] = {0}, plant_id;
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
        gamevar.frog_bullets[i].y = FREE_ENTITY; // Mark as free each frog bullet
    }

    // ALLOCATING PLANTS AND BULLETS FOR THE START
    alloc(Position, gamevar.plants, N_PLANTS);
    alloc(Position*, gamevar.plants_bullets, N_PLANTS);
    for(i = 0; i < N_PLANTS; i++) {
        gamevar.plants[i].y = INCOMING_ENTITY; // Mark as incoming each plant
        alloc(Position, gamevar.plants_bullets[i], MAX_BULLETS_PER_PLANT);
        for(j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
            gamevar.plants_bullets[i][j].y = FREE_ENTITY; // Mark as free the bullets of each plant
        }
    }

    // ALLOCATING CROCCODILES FOR THE START
    alloc(Position*, gamevar.croccodiles, N_WATER_STREAM);
    alloc(bool*, gamevar.bad_croccodiles, N_WATER_STREAM);
    for(i = 0; i < N_WATER_STREAM; i++) { // Croccodile array per stream
        alloc(Position, gamevar.croccodiles[i], MAX_CROCCODILE_PER_STREAM);
        alloc(bool, gamevar.bad_croccodiles[i], MAX_CROCCODILE_PER_STREAM);
        gamevar.croccodiles[i][0].y = INCOMING_ENTITY; // Mark as incoming the first croccodile of each stream
        for(j = 1; j < MAX_CROCCODILE_PER_STREAM; j++) {
            gamevar.croccodiles[i][j].y = FREE_ENTITY; // Mark as free the other croccodiles of each stream
        }
    }

    // Erase old game or demo & print all elements of the game
    wclear(main_scr);
    print_game(&gamevar);
    wrefresh(main_scr);

    // Manche loop
    while(!manche_ended) {
        
        // Read from pipe
        read_msg(pipe_fds[PIPE_READ], &msg);

        switch(msg.id) {

            // RESIZE AND AUTO-PAUSE
            case RESIZE_ID:
                // Check the current time with the last continue to prevent multiple resize message at once
                if(timestamp() - resize_time < RESIZE_TIME_THRESHOLD) {
                    break;
                }
                // Put game in pause (resize_proc will be called by menu)
            // PAUSE || CLOSE
            case PAUSE_ID:
            case CLOSE_ID:
                signal_all(process_pids, SIGSTOP); // Pausing all child processes
                if(msg.id == CLOSE_ID) {
                    i = quit_menu(&gamevar);    
                    switch(i) {
                        case NO_ID:
                            break;
                        
                        case YES_ID:
                            gamevar.timer = MANCHE_CLOSE;
                            manche_ended = TRUE;
                            break;
                    }
                } else {
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
                if(msg.sig == FROG_SHOT_SIG) { // If frog shot a bullet
                    if(gamevar.frog_bullets[next_frog_bullet].y == FREE_ENTITY) { // If a frog bullet is free...
                        // Calc bullet params
                        fork_params[BULLET_ID_INDEX] = next_frog_bullet + MIN_FROG_BULLET_ID;
                        fork_params[BULLET_Y_INDEX] = gamevar.frog.y - 1;
                        fork_params[BULLET_X_INDEX] = gamevar.frog.x + FROG_DIM_X/2;
                        // Fork
                        async_exec(pipe_fds, &process_pids, fork_params[BULLET_ID_INDEX], &bullet_process, fork_params);
                        // Update next bullet & counter
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
                        entity_stream = (gamevar.frog.y - LINE_RIVER) / FROG_DIM_Y;
                        entity_id = gamevar.frog_on_croccodile - MIN_CROCCODILE_ID - entity_stream*MAX_CROCCODILE_PER_STREAM;
                        if(gamevar.bad_croccodiles[entity_stream][entity_id]) {
                            restore_color = GREEN_BORDEAUX; // If frog was on bad croccodile set bordeaux
                        } else {
                            restore_color = GREEN_DARKGREEN; // If frog was on good croccodile set dark green
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

                // If the frog is in bank 1 and it isn't in front of a trap, manche is lost
                if(gamevar.frog.y < LINE_BANK_1) {
                    for(i = 0; i < N_HOLES; i++) {
                        if(gamevar.frog.x >= i*MAIN_COLS/N_HOLES + (MAIN_COLS/N_HOLES - HOLE_DIM_X)/2 && gamevar.frog.x <= i*MAIN_COLS/N_HOLES + (MAIN_COLS/N_HOLES - HOLE_DIM_X)/2 + HOLE_DIM_X - FROG_DIM_X && gamevar.holes_occupied[i] == FALSE) {
                            manche_ended = TRUE;
                            gamevar.holes_occupied[i] = TRUE;
                            break;
                        }
                    }
                    if(!manche_ended) {
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                    }
                }

                gamevar.frog_on_croccodile = FROG_NOT_ON_CROCCODILE; // Reset croccodile index with frog on it
                if(gamevar.frog.y >= LINE_RIVER) {
                    // Frog collision with river
                    if(gamevar.frog.y < LINE_BANK_2) { // If frog is in river zone...
                        entity_stream = (gamevar.frog.y - LINE_RIVER) / FROG_DIM_Y;
                        for(i = 0; i < MAX_CROCCODILE_PER_STREAM; i++) { // Check if frog is on a existing croccodile
                            if(gamevar.croccodiles[entity_stream][i].y >= 0 && gamevar.frog.x >= gamevar.croccodiles[entity_stream][i].x && gamevar.frog.x <= gamevar.croccodiles[entity_stream][i].x + CROCCODILE_DIM_X - FROG_DIM_X) {
                                gamevar.frog_on_croccodile = i + entity_stream*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
                                kill(process_pids.list[gamevar.frog_on_croccodile], FROG_ON_CROCCODILE_SIG);
                                break;
                            }
                        }
                        if(gamevar.frog_on_croccodile == FROG_NOT_ON_CROCCODILE) { // If frog isn't on a croccodile: manche lost
                            gamevar.timer = MANCHE_LOST;
                            manche_ended = TRUE;
                        }
                    }

                    // Frog collision with plant bullets
                    for(i = 0; i < N_PLANTS; i++) {
                        for(j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                            if(gamevar.plants_bullets[i][j].y + BULLET_DIM_Y > gamevar.frog.y && gamevar.plants_bullets[i][j].y < gamevar.frog.y + FROG_DIM_Y && gamevar.plants_bullets[i][j].x + BULLET_DIM_X > gamevar.frog.x && gamevar.plants_bullets[i][j].x < gamevar.frog.x + FROG_DIM_X) {
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                                break;
                            }
                        }
                    }
                } else {
                    // Frog collision with plants
                    for(i = 0; i < N_PLANTS; i++) {
                        if(gamevar.frog.x + FROG_DIM_X > gamevar.plants[i].x && gamevar.frog.x < gamevar.plants[i].x + PLANT_DIM_X) {
                            gamevar.timer = MANCHE_LOST;
                            manche_ended = TRUE;
                            break;
                        }
                    }
                }

                // Frog collision with frog bullets
                for(i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                    if(gamevar.frog.x + FROG_DIM_X > gamevar.frog_bullets[i].x && gamevar.frog.x < gamevar.frog_bullets[i].x + BULLET_DIM_X && gamevar.frog.y + FROG_DIM_Y > gamevar.frog_bullets[i].y && gamevar.frog.y < gamevar.frog_bullets[i].y + BULLET_DIM_Y) {
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                        break;
                    }
                }

                // Print frog
                print_frog(&gamevar);

                break;

            default:

            // CROCCODILE
            if(msg.id >= MIN_CROCCODILE_ID && msg.id < MIN_PLANT_ID) {
                entity_stream = (msg.y - LINE_RIVER) / FROG_DIM_Y;
                entity_id = msg.id - MIN_CROCCODILE_ID - entity_stream*MAX_CROCCODILE_PER_STREAM;
                gamevar.bad_croccodiles[entity_stream][entity_id] = (bool) msg.sig;

                // De-print croccodile
                if(gamevar.croccodiles[entity_stream][entity_id].y >= 0) { // If croccodile is not free or incoming...
                    if(gamevar.croccodiles[entity_stream][entity_id].x < 0) {
                        restore_croccodile_x = 0;
                        restore_croccodile_len = CROCCODILE_DIM_X + gamevar.croccodiles[entity_stream][entity_id].x;
                    } else if(gamevar.croccodiles[entity_stream][entity_id].x < MAIN_COLS - CROCCODILE_DIM_X) {
                        restore_croccodile_x = gamevar.croccodiles[entity_stream][entity_id].x;
                        restore_croccodile_len = CROCCODILE_DIM_X;
                    } else {
                        restore_croccodile_x = gamevar.croccodiles[entity_stream][entity_id].x;
                        restore_croccodile_len = MAIN_COLS - gamevar.croccodiles[entity_stream][entity_id].x;
                    }
                    for(i = 0; i < CROCCODILE_DIM_Y; i++) {
                        mvwaprintw(main_scr, i + gamevar.croccodiles[entity_stream][entity_id].y, restore_croccodile_x, RIVER_BG, "%*s", restore_croccodile_len, "");
                    }
                }

                // Print croccodile or free it
                if(msg.x <= -CROCCODILE_DIM_X || msg.x >= MAIN_COLS || msg.sig == CROCCODILE_IMMERSION_SIG) { // If croccodile is out of screen...
                    gamevar.croccodiles[entity_stream][entity_id].y = FREE_ENTITY; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died croccodile process
                    process_pids.list[msg.id] = 0;
                } else {
                    gamevar.croccodiles[entity_stream][entity_id].x = msg.x;
                    gamevar.croccodiles[entity_stream][entity_id].y = msg.y;
                    print_croccodile(gamevar.croccodiles[entity_stream][entity_id], stream_speed[entity_stream], msg.sig);

                    // Check if frog is on top
                    if(gamevar.frog_on_croccodile == msg.id) {
                        if(msg.sig == CROCCODILE_IMMERSION_SIG) {
                            manche_ended = TRUE;
                            gamevar.timer = MANCHE_LOST;
                        } else if(stream_speed[entity_stream] > 0) { // Update frog X position
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

                        // Frog collision with plants bullets
                        for(i = 0; i < N_PLANTS; i++) {
                            for(j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                                if(gamevar.plants_bullets[i][j].y + BULLET_DIM_Y > gamevar.frog.y && gamevar.plants_bullets[i][j].y < gamevar.frog.y + FROG_DIM_Y && gamevar.plants_bullets[i][j].x + BULLET_DIM_X > gamevar.frog.x && gamevar.plants_bullets[i][j].x < gamevar.frog.x + FROG_DIM_X) {
                                    gamevar.timer = MANCHE_LOST;
                                    manche_ended = TRUE;
                                    break;
                                }
                            }
                        }

                        // Frog collision with frog bullets
                        for(i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                            if(gamevar.frog.x + FROG_DIM_X > gamevar.frog_bullets[i].x && gamevar.frog.x < gamevar.frog_bullets[i].x + BULLET_DIM_X && gamevar.frog.y + FROG_DIM_Y > gamevar.frog_bullets[i].y && gamevar.frog.y < gamevar.frog_bullets[i].y + BULLET_DIM_Y) {
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                                break;
                            }
                        }

                        // Print frog
                        print_frog(&gamevar);

                    }
                    // Croccodile collision with frog bullets
                    for(i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                        if(gamevar.frog_bullets[i].x + BULLET_DIM_X > msg.x && gamevar.frog_bullets[i].x < msg.x + CROCCODILE_DIM_X && gamevar.frog_bullets[i].y + BULLET_DIM_Y > msg.y && gamevar.frog_bullets[i].y < msg.y + CROCCODILE_DIM_Y) {
                            // Update counter
                            gamevar.free_frog_bullet++;
                            print_free_frog_bullet(gamevar.free_frog_bullet);
                            gamevar.frog_bullets[i].y = FREE_ENTITY; // Mark it as free
                            kill(process_pids.list[MIN_FROG_BULLET_ID + i], SIGKILL); // Kill bullet process
                            waitpid(process_pids.list[MIN_FROG_BULLET_ID + i], NULL, 0); // Handle died bullet process
                            process_pids.list[MIN_FROG_BULLET_ID + i] = 0;
                            // Change kindness of croccodile if it is bad
                            if(gamevar.bad_croccodiles[entity_stream][entity_id] == TRUE) {
                                kill(process_pids.list[msg.id], GOOD_CROCCODILE_SIG);
                            }
                            break;
                        }
                    }

                    // Check if needs to spawn another croccodile (if last croccodile is completely in screen)
                    if(stream_last[entity_stream] == entity_id && (stream_speed[entity_stream] > 0 ? (msg.x > 0) : (msg.x < MAIN_COLS - CROCCODILE_DIM_X))) {
                        next_croccodile_id = mod(entity_id+1, MAX_CROCCODILE_PER_STREAM);
                        if(gamevar.croccodiles[entity_stream][next_croccodile_id].y == FREE_ENTITY) { // Check if next croccodile in same stream is free
                            gamevar.croccodiles[entity_stream][next_croccodile_id].y = INCOMING_ENTITY; // Mark the founded free croccodile as incoming
                            fork_params[CROCCODILE_ID_INDEX] = next_croccodile_id + entity_stream*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
                            fork_params[CROCCODILE_STREAM_INDEX] = entity_stream;
                            fork_params[CROCCODILE_SPEED_INDEX] = stream_speed[entity_stream];
                            async_exec(pipe_fds, &process_pids, fork_params[CROCCODILE_ID_INDEX], &croccodile_process, fork_params); // Calls the fork for croccodile process handling the errors
                            stream_last[entity_stream] = next_croccodile_id; // Update last croccodile of stream
                        }
                    }
                }
            }

            // PLANT
            else if(msg.id >= MIN_PLANT_ID && msg.id < MIN_FROG_BULLET_ID) {
                entity_id = msg.id - MIN_PLANT_ID;
                if(msg.sig == PLANT_SPAWN_SIG) { // Spawn
                    gamevar.plants[entity_id].y = msg.y;
                    gamevar.plants[entity_id].x = msg.x;
                    print_plant(gamevar.plants[entity_id]);
                } else if(msg.sig == PLANT_SHOT_SIG) { // Shot
                    if(gamevar.plants_bullets[entity_id][next_plant_bullet[entity_id]].y == FREE_ENTITY) {
                        // Calc bullet params
                        fork_params[BULLET_ID_INDEX] = next_plant_bullet[entity_id] + MAX_BULLETS_PER_PLANT*entity_id + MIN_PLANT_BULLET_ID;
                        fork_params[BULLET_Y_INDEX] = gamevar.plants[entity_id].y + PLANT_DIM_Y;
                        fork_params[BULLET_X_INDEX] = gamevar.plants[entity_id].x + PLANT_DIM_X/2;
                        async_exec(pipe_fds, &process_pids, fork_params[BULLET_ID_INDEX], &bullet_process, fork_params); // Fork
                        next_plant_bullet[entity_id] = mod(next_plant_bullet[entity_id] + 1, MAX_BULLETS_PER_PLANT); // Update next
                    }
                }
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

                if(msg.y < LINE_BANK_1) { // If frog bullet is out of screen...
                    gamevar.free_frog_bullet++;
                    print_free_frog_bullet(gamevar.free_frog_bullet);
                    gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                    process_pids.list[msg.id] = 0;
                } else {
                    // Update coordinates & print
                    gamevar.frog_bullets[entity_id].y = msg.y;
                    gamevar.frog_bullets[entity_id].x = msg.x;
                    print_bullet(gamevar.frog_bullets[entity_id]);

                    if(msg.y >= LINE_RIVER) {
                        entity_stream = (msg.y - LINE_RIVER) / FROG_DIM_Y;
                        // Frog bullet collision with croccodile
                        for(i = 0; i < MAX_CROCCODILE_PER_STREAM; i++) {
                            if(msg.x + BULLET_DIM_X > gamevar.croccodiles[entity_stream][i].x && msg.x < gamevar.croccodiles[entity_stream][i].x + CROCCODILE_DIM_X) {
                                // Update counter
                                gamevar.free_frog_bullet++;
                                print_free_frog_bullet(gamevar.free_frog_bullet);
                                gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                                kill(process_pids.list[msg.id], SIGKILL);
                                waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                                process_pids.list[msg.id] = 0;
                                // Change kindness of croccodile if it is bad
                                if(gamevar.bad_croccodiles[entity_stream][i] == TRUE) {
                                    kill(process_pids.list[MIN_CROCCODILE_ID + entity_stream*MAX_CROCCODILE_PER_STREAM + i], GOOD_CROCCODILE_SIG);
                                }
                                break;
                            }
                        }
                        // Frog bullet collision with plant bullet
                        for(i = 0; i < N_PLANTS; i++) {
                            for(j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                                if(msg.x + BULLET_DIM_X > gamevar.plants_bullets[i][j].x && msg.x < gamevar.plants_bullets[i][j].x + BULLET_DIM_X && msg.y + BULLET_DIM_Y > gamevar.plants_bullets[i][j].y && msg.y < gamevar.plants_bullets[i][j].y + BULLET_DIM_Y) {
                                    // Update counter
                                    gamevar.free_frog_bullet++;
                                    print_free_frog_bullet(gamevar.free_frog_bullet);
                                    // Free frog bullet
                                    gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                                    kill(process_pids.list[msg.id], SIGKILL);
                                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                                    process_pids.list[msg.id] = 0;
                                    // Free plant bullet
                                    gamevar.plants_bullets[i][j].y = FREE_ENTITY; // Mark it as free
                                    kill(process_pids.list[MIN_PLANT_BULLET_ID + MAX_BULLETS_PER_PLANT*i + j], SIGKILL);
                                    waitpid(process_pids.list[MIN_PLANT_BULLET_ID + MAX_BULLETS_PER_PLANT*i + j], NULL, 0); // Handle died bullet process
                                    process_pids.list[MIN_PLANT_BULLET_ID + MAX_BULLETS_PER_PLANT*i + j] = 0;
                                    // De-print
                                    for(int k = 0; k < BULLET_DIM_Y; k++) {
                                        mvwaprintw(main_scr, msg.y + k, msg.x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                                    }
                                    break;
                                }
                            }
                        }
                    } else {
                        // Frog bullet collision with plant
                        for(i = 0; i < N_PLANTS; i++) {
                            if(msg.x + BULLET_DIM_X > gamevar.plants[i].x && msg.x < gamevar.plants[i].x + PLANT_DIM_X) {
                                // Update counter
                                gamevar.free_frog_bullet++;
                                print_free_frog_bullet(gamevar.free_frog_bullet);
                                // Free frog bullet
                                gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                                kill(process_pids.list[msg.id], SIGKILL);
                                waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                                process_pids.list[msg.id] = 0;
                                // De-print plant
                                for(j = 0; j < PLANT_DIM_Y; j++) {
                                    mvwaprintw(main_scr, gamevar.plants[i].y + j, gamevar.plants[i].x, BANK_BG, "%*s", PLANT_DIM_X, "");
                                }
                                // Kill plant & spawn another
                                kill(process_pids.list[MIN_PLANT_ID + i], SIGKILL);
                                waitpid(process_pids.list[MIN_PLANT_ID + i], NULL, 0); // Handle died plant process
                                gamevar.plants[i].y = INCOMING_ENTITY;
                                fork_params[PLANT_ID_INDEX] = MIN_PLANT_ID + i;
                                do {
                                    occupied_plant = FALSE;
                                    fork_params[PLANT_X_INDEX] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
                                    for(j = 0; j < N_PLANTS; j++) {
                                        if(fork_params[PLANT_X_INDEX] > gamevar.plants[j].x - PLANT_DIM_X && fork_params[PLANT_X_INDEX] < gamevar.plants[j].x + PLANT_DIM_X && j != i) {
                                            occupied_plant = TRUE;
                                            break;
                                        }
                                    }
                                } while(occupied_plant);
                                async_exec(pipe_fds, &process_pids, fork_params[PLANT_ID_INDEX], &plant_process, fork_params);
                                break;
                            }
                        }
                    }
                    // Frog bullet collision with frog
                    if(gamevar.frog.x + FROG_DIM_X > msg.x && gamevar.frog.x < msg.x - BULLET_DIM_X && gamevar.frog.y + FROG_DIM_Y > msg.y && gamevar.frog.y < msg.y - BULLET_DIM_Y) {
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                    }
                }
            }

            // PLANT BULLET
            else if(msg.id >= MIN_PLANT_BULLET_ID && msg.id < LIM_N_ENTITIES) {
                plant_id = (msg.id - MIN_PLANT_BULLET_ID) / MAX_BULLETS_PER_PLANT;
                entity_id = msg.id - MIN_PLANT_BULLET_ID - plant_id*MAX_BULLETS_PER_PLANT;

                // De-print
                if(gamevar.plants_bullets[plant_id][entity_id].y >= 0) {
                    for(i = 0; i < BULLET_DIM_Y; i++) {
                        mvwaprintw(main_scr, gamevar.plants_bullets[plant_id][entity_id].y, gamevar.plants_bullets[plant_id][entity_id].x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                    }
                }

                if(msg.y >= MAIN_ROWS) { // If frog bullet is out of screen...
                    gamevar.plants_bullets[plant_id][entity_id].y = FREE_ENTITY; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                    process_pids.list[msg.id] = 0;
                } else {
                    gamevar.plants_bullets[plant_id][entity_id].y = msg.y;
                    gamevar.plants_bullets[plant_id][entity_id].x = msg.x;
                    print_bullet(gamevar.plants_bullets[plant_id][entity_id]);
                    
                    // Plant bullet collision with frog
                    if(msg.x + BULLET_DIM_X > gamevar.frog.x && msg.x < gamevar.frog.x + FROG_DIM_X && msg.y + BULLET_DIM_Y >= gamevar.frog.y && msg.y <= gamevar.frog.y + FROG_DIM_Y) {
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                    }
                    // Plant bullet collision with frog bullet
                    for(i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                        if(msg.x + BULLET_DIM_X > gamevar.frog_bullets[i].x && msg.x < gamevar.frog_bullets[i].x + BULLET_DIM_X && msg.y + BULLET_DIM_Y > gamevar.frog_bullets[i].y && msg.y < gamevar.frog_bullets[i].y + BULLET_DIM_Y) {
                            gamevar.free_frog_bullet++;
                            print_free_frog_bullet(gamevar.free_frog_bullet);
                            gamevar.frog_bullets[i].y = FREE_ENTITY; // Mark it as free
                            gamevar.plants_bullets[plant_id][entity_id].y = FREE_ENTITY; // Mark it as free
                            kill(process_pids.list[MIN_FROG_BULLET_ID + i], SIGKILL);
                            waitpid(process_pids.list[MIN_FROG_BULLET_ID + i], NULL, 0); // Handle died bullet process
                            process_pids.list[MIN_FROG_BULLET_ID + i] = 0;
                            kill(process_pids.list[msg.id], SIGKILL);
                            waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                            process_pids.list[msg.id] = 0;
                            // De-print
                            for(i = 0; i < BULLET_DIM_Y; i++) {
                                mvwaprintw(main_scr, msg.y, msg.x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                            }
                            break;
                        }
                    }
                    
                }
            }

            break;
        }
        
        // Refresh
        if(timestamp() - refresh_time > REFRESH_TIME_THRESHOLD) {
            wrefresh(main_scr);
        }
    }
    wrefresh(main_scr);
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
