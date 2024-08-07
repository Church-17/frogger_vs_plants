// Include libs
#include <unistd.h>
#include "main.h"
#include "menu.h"
#include "manche.h"
#include "music.h"
#include "utils.h"
#include "struct.h"
#include "entity.h"
#include "process.h"

// Define constant
#define RESIZE_TIME_THRESHOLD 100
#define REFRESH_TIME_THRESHOLD 10

// Play a manche, return game vars with in gamevar.timer the time remaining or a manche_id
Game_t play_manche(int score, int n_lifes, bool* holes_occupied) {
    // Init vars
    bool occupied_plant;
    int id, process_params[LIM_N_ENTITIES][N_ENTITY_PARAMS], stream_speed[N_WATER_STREAM], plants_x[N_PLANTS];
    pid_t array_pids[LIM_N_ENTITIES] = {0}; // Pids for every process
    List_pid process_pids = {array_pids, LIM_N_ENTITIES};
    
    // Pipe
    int pipe_fds[PIPE_DIM];
    if(pipe(pipe_fds) != 0) {
        quit(ERR_PIPE);
    }

    // Forks
    async_exec(pipe_fds, &process_pids, FROG_ID, &frog_process, NULL); // Calls the fork for frog process handling the errors
    async_exec(pipe_fds, &process_pids, TIME_ID, &time_process, NULL); // Calls the fork for time process handling the errors
    for(int i = 0; i < N_WATER_STREAM; i++) {
        // Randomize speed & direction of each stream
        stream_speed[i] = rand_range(MIN_STREAM_SPEED, MAX_STREAM_SPEED) * (rand_range(0, 2) ? 1 : -1);
        // Write croccodile params
        id = i*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
        process_params[id][CROCCODILE_ID_INDEX] = id;
        process_params[id][CROCCODILE_STREAM_INDEX] = i;
        process_params[id][CROCCODILE_SPEED_INDEX] = stream_speed[i];
        // Calls the fork for croccodile process handling the errors
        async_exec(pipe_fds, &process_pids, id, &croccodile_process, process_params[id]);
    }
    for(int i = 0; i < N_PLANTS; i++) {
        id = MIN_PLANT_ID + i;
        process_params[id][PLANT_ID_INDEX] = id;
        do { // Randomize X coordinates of plant
            occupied_plant = FALSE;
            plants_x[i] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
            for(int j = 0; j < i; j++) {
                // Check if any plants if anyready in that space
                if(plants_x[i] + PLANT_DIM_X > plants_x[j] && plants_x[i] < plants_x[j] + PLANT_DIM_X) {
                    occupied_plant = TRUE;
                    break;
                }
            }
        } while(occupied_plant);
        process_params[id][PLANT_X_INDEX] = plants_x[i];
        async_exec(pipe_fds, &process_pids, id, &plant_process, process_params[id]);
    }
    
    // --- PARENT PROCESS ---

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int entity_stream, entity_id, next_croccodile_id, restore_croccodile_x, restore_croccodile_len, plant_id; // Helper vars
    int stream_last[N_WATER_STREAM] = {0}; // Track which croccodile was the last of each stream
    time_t resize_time = 0, refresh_time = 0; // Var to store time of the last continue to prevent multiple resize message at once
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
    for(int i = 0; i < MAX_BULLETS_PER_FROG; i++) {
        gamevar.frog_bullets[i].y = FREE_ENTITY; // Mark as free each frog bullet
    }

    // Allocating plants and bullets for the start
    alloc(Position, gamevar.plants, N_PLANTS);
    alloc(int, gamevar.plants_sig, N_PLANTS);
    alloc(Position*, gamevar.plants_bullets, N_PLANTS);
    for(int i = 0; i < N_PLANTS; i++) {
        gamevar.plants[i].y = INCOMING_ENTITY; // Mark as incoming each plant
        gamevar.plants[i].x = plants_x[i]; // Write plant x coordinate
        alloc(Position, gamevar.plants_bullets[i], MAX_BULLETS_PER_PLANT);
        for(int j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
            gamevar.plants_bullets[i][j].y = FREE_ENTITY; // Mark as free the bullets of each plant
        }
    }

    // Allocating croccodiles for the start
    alloc(Position*, gamevar.croccodiles, N_WATER_STREAM);
    alloc(int*, gamevar.croccodiles_kind, N_WATER_STREAM);
    for(int i = 0; i < N_WATER_STREAM; i++) { // Croccodile array per stream
        alloc(Position, gamevar.croccodiles[i], MAX_CROCCODILE_PER_STREAM);
        alloc(int, gamevar.croccodiles_kind[i], MAX_CROCCODILE_PER_STREAM);
        gamevar.croccodiles[i][0].y = INCOMING_ENTITY; // Mark as incoming the first croccodile of each stream
        for(int j = 1; j < MAX_CROCCODILE_PER_STREAM; j++) {
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
        msg = read_msg(pipe_fds[PIPE_READ]);

        switch(msg.id) {

            // RESIZE AND AUTO-PAUSE
            case RESIZE_ID:
                // Check the current time with the last continue to prevent multiple resize message at once
                if(timestamp() - resize_time < RESIZE_TIME_THRESHOLD) {
                    break;
                }
                ungetch(KEY_RESIZE); // Push KEY_RESIZE in all getch() buffer
                // Put game in pause (resize_proc will be called by menu caused by KEY_RESIZE pushed)
            // PAUSE || CLOSE
            case PAUSE_ID:
            case CLOSE_ID:
                pause_music(); // Pause music
                play_sound(SOUND_MANCHE_WON); // Play pause sound
                signal_all(process_pids, SIGSTOP); // Pause all child processes
                if(msg.id == CLOSE_ID) {
                    entity_id = quit_menu(&gamevar);    
                    switch(entity_id) {
                        case NO_ID:
                            resume_music(); // Resume music
                            print_game(&gamevar); // Redraw game
                            signal_all(process_pids, SIGCONT); // Resume all child processes
                            resize_time = timestamp(); // Save the current time to prevent multiple resize message at once
                            break;
                        
                        case YES_ID:
                            gamevar.timer = MANCHE_CLOSE;
                            manche_ended = TRUE;
                            break;
                    }
                } else {
                    entity_id = pause_menu(&gamevar);
                    switch(entity_id) {
                        case PAUSE_RES_ID:
                            resume_music(); // Resume music
                            print_game(&gamevar); // Redraw game
                            signal_all(process_pids, SIGCONT); // Resume all child processes
                            resize_time = timestamp(); // Save the current time to prevent multiple resize message at once
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
                    for(int i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                        if(gamevar.frog_bullets[i].y == FREE_ENTITY) { // If a frog bullet is free...
                            gamevar.frog_bullets[i].y = INCOMING_ENTITY;
                            // Calc bullet params
                            id = i + MIN_FROG_BULLET_ID;
                            process_params[id][BULLET_ID_INDEX] = id;
                            process_params[id][BULLET_Y_INDEX] = gamevar.frog.y - 1;
                            process_params[id][BULLET_X_INDEX] = gamevar.frog.x + FROG_DIM_X/2;
                            // Fork
                            async_exec(pipe_fds, &process_pids, id, &bullet_process, process_params[id]);
                            // Update next bullet & counter
                            gamevar.free_frog_bullet--;
                            print_free_frog_bullet(gamevar.free_frog_bullet);
                            break;
                        }
                    }
                    break;
                }

                // De-print frog
                if(gamevar.frog.y < LINE_RIVER || gamevar.frog.y >= LINE_BANK_2) {
                    for(int i = 0; i < FROG_DIM_Y; i++) {
                        mvwaprintw(main_scr, i + gamevar.frog.y, gamevar.frog.x, GREEN_PURPLE, "%*s", FROG_DIM_X, "");
                    }
                } else { // If frog was on a croccodile...
                    entity_stream = (gamevar.frog.y - LINE_RIVER) / FROG_DIM_Y;
                    entity_id = gamevar.frog_on_croccodile - MIN_CROCCODILE_ID - entity_stream*MAX_CROCCODILE_PER_STREAM; // Croccodile id on stream
                    for(int i = 0; i < FROG_DIM_Y; i++) {
                        mvwaprintw(main_scr, i + gamevar.frog.y, gamevar.frog.x, GREEN_DARKBLUE, "%*s", FROG_DIM_X, "");
                    }
                    print_croccodile(gamevar.croccodiles[entity_stream][entity_id], stream_speed[entity_stream], gamevar.croccodiles_kind[entity_stream][entity_id]);
                }

                // Update frog Y position
                if(gamevar.frog.y >= FROG_LIM_UP && gamevar.frog.y <= FROG_LIM_DOWN) { // If frog can move...
                    gamevar.frog.y += msg.y; // Update coordinate
                    if(gamevar.frog.y < FROG_LIM_UP) { // If frog is outside limit...
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

                gamevar.frog_on_croccodile = FROG_NOT_ON_CROCCODILE; // Reset croccodile index with frog on it
                if(gamevar.frog.y >= LINE_RIVER) {
                    // Frog collision with river
                    if(gamevar.frog.y < LINE_BANK_2) { // If frog is in river zone...
                        entity_stream = (gamevar.frog.y - LINE_RIVER) / FROG_DIM_Y;
                        for(int i = 0; i < MAX_CROCCODILE_PER_STREAM; i++) { // Check if frog is on a existing croccodile
                            if(gamevar.croccodiles[entity_stream][i].y >= 0 && gamevar.frog.x >= gamevar.croccodiles[entity_stream][i].x && gamevar.frog.x <= gamevar.croccodiles[entity_stream][i].x + CROCCODILE_DIM_X - FROG_DIM_X) {
                                gamevar.frog_on_croccodile = MIN_CROCCODILE_ID + entity_stream*MAX_CROCCODILE_PER_STREAM + i;
                                kill(process_pids.list[gamevar.frog_on_croccodile], FROG_ON_CROCCODILE_SIG);
                                break;
                            }
                        }
                        if(gamevar.frog_on_croccodile == FROG_NOT_ON_CROCCODILE) { // If frog isn't on a croccodile: manche lost
                            gamevar.timer = MANCHE_LOST;
                            manche_ended = TRUE;
                        }
                    }

                    print_frog(&gamevar); // Print frog

                    // Frog collision with plant bullets
                    for(int i = 0; i < N_PLANTS; i++) {
                        for(int j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                            if(gamevar.plants_bullets[i][j].y >= 0 && gamevar.frog.y + FROG_DIM_Y > gamevar.plants_bullets[i][j].y && gamevar.frog.y < gamevar.plants_bullets[i][j].y + BULLET_DIM_Y && gamevar.frog.x + FROG_DIM_X > gamevar.plants_bullets[i][j].x && gamevar.frog.x < gamevar.plants_bullets[i][j].x + BULLET_DIM_X) {
                                print_bullet(gamevar.plants_bullets[i][j]);
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    }
                } else if(gamevar.frog.y >= LINE_BANK_1) {
                    // Frog collision with plants
                    for(int i = 0; i < N_PLANTS; i++) {
                        if(gamevar.plants[i].y >= 0 && gamevar.frog.x + FROG_DIM_X > gamevar.plants[i].x && gamevar.frog.x < gamevar.plants[i].x + PLANT_DIM_X) {
                            gamevar.timer = MANCHE_LOST;
                            manche_ended = TRUE;
                            break;
                        }
                    }

                    print_frog(&gamevar); // Print frog

                    // Frog collision with frog bullets
                    for(int i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                        if(gamevar.frog_bullets[i].y >= 0 && gamevar.frog_bullets[i].y < LINE_RIVER && gamevar.frog.x + FROG_DIM_X > gamevar.frog_bullets[i].x && gamevar.frog.x < gamevar.frog_bullets[i].x + BULLET_DIM_X) {
                            print_bullet(gamevar.frog_bullets[i]);
                            gamevar.timer = MANCHE_LOST;
                            manche_ended = TRUE;
                        }
                    }
                } else {
                    // If the frog is in hedge, manche is lost, otherwise manche is won
                    print_frog(&gamevar); // Print frog
                    
                    for(int i = 0; i < N_HOLES; i++) {
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

                break;

            default:

            // CROCCODILE
            if(msg.id >= MIN_CROCCODILE_ID && msg.id < MIN_PLANT_ID) {
                entity_stream = (msg.y - LINE_RIVER) / FROG_DIM_Y;
                entity_id = msg.id - MIN_CROCCODILE_ID - entity_stream*MAX_CROCCODILE_PER_STREAM;

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
                    for(int i = 0; i < CROCCODILE_DIM_Y; i++) {
                        mvwaprintw(main_scr, i + gamevar.croccodiles[entity_stream][entity_id].y, restore_croccodile_x, GREEN_DARKBLUE, "%*s", restore_croccodile_len, "");
                    }
                }

                // Free croccodile if it is disappeared
                if(msg.x <= -CROCCODILE_DIM_X || msg.x >= MAIN_COLS || msg.sig == CROCCODILE_IMMERSION_SIG) { // If croccodile is out of screen...
                    gamevar.croccodiles[entity_stream][entity_id].y = FREE_ENTITY; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died croccodile process
                    process_pids.list[msg.id] = 0;
                    if(gamevar.frog_on_croccodile == msg.id) {
                        manche_ended = TRUE;
                        gamevar.timer = MANCHE_LOST;
                    }
                    break;
                }

                // Update coordinates & kindness
                gamevar.croccodiles[entity_stream][entity_id].x = msg.x;
                gamevar.croccodiles[entity_stream][entity_id].y = msg.y;
                gamevar.croccodiles_kind[entity_stream][entity_id] = msg.sig;
                print_croccodile(gamevar.croccodiles[entity_stream][entity_id], stream_speed[entity_stream], msg.sig);

                // Check if frog is on top
                if(gamevar.frog_on_croccodile == msg.id) {
                    // Update frog X position
                    if(stream_speed[entity_stream] > 0) {
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
                    print_frog(&gamevar); // Print frog

                    // Frog on croccodile collision with plants bullets
                    for(int i = 0; i < N_PLANTS; i++) {
                        for(int j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                            if(gamevar.plants_bullets[i][j].y >= 0 && gamevar.frog.y + FROG_DIM_Y > gamevar.plants_bullets[i][j].y && gamevar.frog.y < gamevar.plants_bullets[i][j].y + BULLET_DIM_Y && gamevar.frog.x + FROG_DIM_X > gamevar.plants_bullets[i][j].x && gamevar.frog.x < gamevar.plants_bullets[i][j].x + BULLET_DIM_X) {
                                print_bullet(gamevar.plants_bullets[i][j]);
                                gamevar.timer = MANCHE_LOST;
                                manche_ended = TRUE;
                            }
                        }
                    }
                }

                // Croccodile collision with frog bullets
                for(int i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                    if(gamevar.frog_bullets[i].y >= 0 && msg.y + CROCCODILE_DIM_Y > gamevar.frog_bullets[i].y && msg.y < gamevar.frog_bullets[i].y + BULLET_DIM_Y && msg.x + CROCCODILE_DIM_X > gamevar.frog_bullets[i].x && msg.x < gamevar.frog_bullets[i].x + BULLET_DIM_X) {
                        // Update counter
                        gamevar.free_frog_bullet++;
                        print_free_frog_bullet(gamevar.free_frog_bullet);
                        gamevar.frog_bullets[i].y = FREE_ENTITY; // Mark it as free
                        kill(process_pids.list[MIN_FROG_BULLET_ID + i], SIGKILL); // Kill bullet process
                        waitpid(process_pids.list[MIN_FROG_BULLET_ID + i], NULL, 0); // Handle died bullet process
                        process_pids.list[MIN_FROG_BULLET_ID + i] = 0;
                        if(msg.sig != CROCCODILE_GOOD_SIG) { // Change kindness of croccodile if it is bad
                            kill(process_pids.list[msg.id], CROCCODILE_SHOTTED_SIG);
                        }
                    }
                }

                // Croccodile collision with plant bullet
                for(int i = 0; i < N_PLANTS; i++) {
                    for(int j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                        if(gamevar.plants_bullets[i][j].y >= 0 && msg.y + CROCCODILE_DIM_Y > gamevar.plants_bullets[i][j].y && msg.y < gamevar.plants_bullets[i][j].y + BULLET_DIM_Y && msg.x + CROCCODILE_DIM_X > gamevar.plants_bullets[i][j].x && msg.x < gamevar.plants_bullets[i][j].x + BULLET_DIM_X) {
                            print_bullet(gamevar.plants_bullets[i][j]);
                        }
                    }
                }

                // Check if needs to spawn another croccodile (if last croccodile is completely in screen)
                if(stream_last[entity_stream] == entity_id && (stream_speed[entity_stream] > 0 ? (msg.x > 0) : (msg.x < MAIN_COLS - CROCCODILE_DIM_X))) {
                    next_croccodile_id = mod(entity_id+1, MAX_CROCCODILE_PER_STREAM);
                    if(gamevar.croccodiles[entity_stream][next_croccodile_id].y == FREE_ENTITY) { // Check if next croccodile in same stream is free
                        gamevar.croccodiles[entity_stream][next_croccodile_id].y = INCOMING_ENTITY; // Mark the founded free croccodile as incoming
                        id = next_croccodile_id + entity_stream*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
                        process_params[id][CROCCODILE_ID_INDEX] = id;
                        process_params[id][CROCCODILE_STREAM_INDEX] = entity_stream;
                        process_params[id][CROCCODILE_SPEED_INDEX] = stream_speed[entity_stream];
                        async_exec(pipe_fds, &process_pids, id, &croccodile_process, process_params[id]); // Calls the fork for croccodile process handling the errors
                        stream_last[entity_stream] = next_croccodile_id; // Update last croccodile of stream
                    }
                }
            }

            // PLANT
            else if(msg.id >= MIN_PLANT_ID && msg.id < MIN_FROG_BULLET_ID) {
                entity_id = msg.id - MIN_PLANT_ID;
                gamevar.plants_sig[entity_id] = msg.sig;

                if(msg.sig == PLANT_SPAWN_SIG) { // Spawn
                    // If plant spawn on frog, kill plant & spawn another
                    if(gamevar.frog.y == LINE_BANK_1 && msg.x + PLANT_DIM_X > gamevar.frog.x && msg.x < gamevar.frog.x + FROG_DIM_X) {
                        kill(process_pids.list[msg.id], SIGKILL);
                        waitpid(process_pids.list[msg.id], NULL, 0); // Handle died plant process
                        gamevar.plants[entity_id].y = INCOMING_ENTITY;
                        process_params[msg.id][PLANT_ID_INDEX] = msg.id;
                        do {
                            occupied_plant = FALSE;
                            process_params[msg.id][PLANT_X_INDEX] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
                            for(int j = 0; j < N_PLANTS; j++) {
                                if(j != entity_id && process_params[msg.id][PLANT_X_INDEX] + PLANT_DIM_X > gamevar.plants[j].x && process_params[msg.id][PLANT_X_INDEX] < gamevar.plants[j].x + PLANT_DIM_X) {
                                    occupied_plant = TRUE;
                                    break;
                                }
                            }
                        } while(occupied_plant);
                        gamevar.plants[entity_id].x = process_params[msg.id][PLANT_X_INDEX];
                        async_exec(pipe_fds, &process_pids, msg.id, &plant_process, process_params[msg.id]);
                        break;
                    }
                    // Update coordinates & print
                    play_sound(SOUND_SPAWN_PLANT);
                    gamevar.plants[entity_id].y = msg.y;
                    print_plant(gamevar.plants[entity_id], msg.sig);
                } else if(msg.sig == PLANT_SHOT_SIG) { // Shot
                    for(int i = 0; i < MAX_BULLETS_PER_PLANT; i++) {
                        if(gamevar.plants_bullets[entity_id][i].y == FREE_ENTITY) {
                            gamevar.plants_bullets[entity_id][i].y = INCOMING_ENTITY;
                            // Calc bullet params
                            id = i + MAX_BULLETS_PER_PLANT*entity_id + MIN_PLANT_BULLET_ID;
                            process_params[id][BULLET_ID_INDEX] = id;
                            process_params[id][BULLET_Y_INDEX] = gamevar.plants[entity_id].y + PLANT_DIM_Y;
                            process_params[id][BULLET_X_INDEX] = gamevar.plants[entity_id].x + PLANT_DIM_X/2;
                            async_exec(pipe_fds, &process_pids, id, &bullet_process, process_params[id]); // Fork
                            break;
                        }
                    }
                } else {
                    print_plant(gamevar.plants[entity_id], msg.sig);
                }
            }

            // FROG BULLET
            else if(msg.id >= MIN_FROG_BULLET_ID && msg.id < MIN_PLANT_BULLET_ID) {
                entity_id = msg.id - MIN_FROG_BULLET_ID;

                // Ignore message from inexistent bullet
                if(gamevar.frog_bullets[entity_id].y == FREE_ENTITY) {
                    break;
                }

                // De-print bullet
                if(gamevar.frog_bullets[entity_id].y >= 0) {
                    if(gamevar.frog_bullets[entity_id].y >= LINE_RIVER) {
                        mvwaprintw(main_scr, gamevar.frog_bullets[entity_id].y, gamevar.frog_bullets[entity_id].x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                    } else {
                        mvwaprintw(main_scr, gamevar.frog_bullets[entity_id].y, gamevar.frog_bullets[entity_id].x, GREEN_PURPLE, "%*s", BULLET_DIM_X, "");
                    }
                } else { // If it's just spwaned play sound
                    play_sound(SOUND_BULLET);
                }

                // If frog bullet is out of limit free it
                if(msg.y < LINE_BANK_1) {
                    gamevar.free_frog_bullet++;
                    print_free_frog_bullet(gamevar.free_frog_bullet);
                    gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                    process_pids.list[msg.id] = 0;
                    break;
                }

                // Update coordinates & print
                gamevar.frog_bullets[entity_id].y = msg.y;
                gamevar.frog_bullets[entity_id].x = msg.x;
                print_bullet(gamevar.frog_bullets[entity_id]);

                if(msg.y >= LINE_RIVER) {
                    entity_stream = (msg.y - LINE_RIVER) / FROG_DIM_Y;

                    // Frog bullet collision with croccodile
                    for(int i = 0; i < MAX_CROCCODILE_PER_STREAM; i++) {
                        if(gamevar.croccodiles[entity_stream][i].y >= 0 && msg.x + BULLET_DIM_X > gamevar.croccodiles[entity_stream][i].x && msg.x < gamevar.croccodiles[entity_stream][i].x + CROCCODILE_DIM_X) {
                            // Update counter
                            gamevar.free_frog_bullet++;
                            print_free_frog_bullet(gamevar.free_frog_bullet);
                            gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                            kill(process_pids.list[msg.id], SIGKILL);
                            waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                            process_pids.list[msg.id] = 0;
                            if(gamevar.croccodiles_kind[entity_stream][i] != CROCCODILE_GOOD_SIG) { // Change kindness of croccodile if it is bad
                                kill(process_pids.list[MIN_CROCCODILE_ID + entity_stream*MAX_CROCCODILE_PER_STREAM + i], CROCCODILE_SHOTTED_SIG);
                            }
                            break;
                        }
                    }
                    if(process_pids.list[msg.id] == 0) {
                        break;
                    }

                    // Frog bullet collision with plant bullet
                    for(int i = 0; i < N_PLANTS; i++) {
                        for(int j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                            if(gamevar.plants_bullets[i][j].y >= 0 && msg.y + BULLET_DIM_Y > gamevar.plants_bullets[i][j].y && msg.y < gamevar.plants_bullets[i][j].y + BULLET_DIM_Y && msg.x + BULLET_DIM_X > gamevar.plants_bullets[i][j].x && msg.x < gamevar.plants_bullets[i][j].x + BULLET_DIM_X) {
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
                                mvwaprintw(main_scr, msg.y, msg.x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                                break;
                            }
                        }
                    }
                } else {
                    // Frog bullet collision with plant
                    for(int i = 0; i < N_PLANTS; i++) {
                        if(gamevar.plants[i].y >= 0 && msg.x + BULLET_DIM_X > gamevar.plants[i].x && msg.x < gamevar.plants[i].x + PLANT_DIM_X) {
                            play_sound(SOUND_DEAD_PLANT);
                            // Update counter
                            gamevar.free_frog_bullet++;
                            print_free_frog_bullet(gamevar.free_frog_bullet);
                            // Free frog bullet
                            gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                            kill(process_pids.list[msg.id], SIGKILL);
                            waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                            process_pids.list[msg.id] = 0;
                            // De-print
                            for(int j = 0; j < PLANT_DIM_Y; j++) {
                                mvwaprintw(main_scr, gamevar.plants[i].y + j, gamevar.plants[i].x, GREEN_PURPLE, "%*s", PLANT_DIM_X, "");
                            }
                            // Kill plant & spawn another
                            id = MIN_PLANT_ID + i;
                            kill(process_pids.list[id], SIGKILL);
                            waitpid(process_pids.list[id], NULL, 0); // Handle died plant process
                            gamevar.plants[i].y = INCOMING_ENTITY;
                            process_params[id][PLANT_ID_INDEX] = id;
                            do {
                                occupied_plant = FALSE;
                                process_params[id][PLANT_X_INDEX] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
                                for(int j = 0; j < N_PLANTS; j++) {
                                    if(j != i && process_params[id][PLANT_X_INDEX] + PLANT_DIM_X > gamevar.plants[j].x && process_params[id][PLANT_X_INDEX] < gamevar.plants[j].x + PLANT_DIM_X) {
                                        occupied_plant = TRUE;
                                        break;
                                    }
                                }
                            } while(occupied_plant);
                            gamevar.plants[i].x = process_params[id][PLANT_X_INDEX];
                            async_exec(pipe_fds, &process_pids, id, &plant_process, process_params[id]);
                            break;
                        }
                    }

                    // Frog bullet collision with frog
                    if(gamevar.frog.y < LINE_RIVER && msg.x + BULLET_DIM_X > gamevar.frog.x && msg.x < gamevar.frog.x + FROG_DIM_X) {
                        gamevar.timer = MANCHE_LOST;
                        manche_ended = TRUE;
                        break;
                    }
                }
            }

            // PLANT BULLET
            else if(msg.id >= MIN_PLANT_BULLET_ID && msg.id < LIM_N_ENTITIES) {
                plant_id = (msg.id - MIN_PLANT_BULLET_ID) / MAX_BULLETS_PER_PLANT;
                entity_id = msg.id - MIN_PLANT_BULLET_ID - plant_id*MAX_BULLETS_PER_PLANT;
                
                // Ignore message from inexistent bullet
                if(gamevar.plants_bullets[plant_id][entity_id].y == FREE_ENTITY) {
                    break;
                }

                // De-print bullet
                if(gamevar.plants_bullets[plant_id][entity_id].y >= 0) {
                    if(gamevar.plants_bullets[plant_id][entity_id].y < LINE_BANK_2) {
                        mvwaprintw(main_scr, gamevar.plants_bullets[plant_id][entity_id].y, gamevar.plants_bullets[plant_id][entity_id].x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                        entity_stream = (gamevar.plants_bullets[plant_id][entity_id].y - LINE_RIVER) / FROG_DIM_Y;
                        for(int i = 0; i < MAX_CROCCODILE_PER_STREAM; i++) {
                            if(gamevar.croccodiles[entity_stream][i].y >= 0 && gamevar.plants_bullets[plant_id][entity_id].x + BULLET_DIM_X > gamevar.croccodiles[entity_stream][i].x && gamevar.plants_bullets[plant_id][entity_id].x < gamevar.croccodiles[entity_stream][i].x + CROCCODILE_DIM_X) {
                                print_croccodile(gamevar.croccodiles[entity_stream][i], stream_speed[entity_stream], gamevar.croccodiles_kind[entity_stream][i]);
                                if(gamevar.frog_on_croccodile == MIN_CROCCODILE_ID + MAX_CROCCODILE_PER_STREAM*entity_stream + i) {
                                    print_frog(&gamevar);
                                }
                                break;
                            }
                        }
                    } else {
                        mvwaprintw(main_scr, gamevar.plants_bullets[plant_id][entity_id].y, gamevar.plants_bullets[plant_id][entity_id].x, GREEN_PURPLE, "%*s", BULLET_DIM_X, "");
                    }
                } else { // If it's just spwaned play sound
                    play_sound(SOUND_BULLET);
                }

                // If frog bullet is out of screen free it
                if(msg.y >= MAIN_ROWS) {
                    gamevar.plants_bullets[plant_id][entity_id].y = FREE_ENTITY; // Mark it as free
                    waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                    process_pids.list[msg.id] = 0;
                    break;
                }

                // Update coordinates
                gamevar.plants_bullets[plant_id][entity_id].y = msg.y;
                gamevar.plants_bullets[plant_id][entity_id].x = msg.x;
                print_bullet(gamevar.plants_bullets[plant_id][entity_id]);
                
                // Plant bullet collision with frog
                if(msg.y + BULLET_DIM_Y > gamevar.frog.y && msg.y < gamevar.frog.y + FROG_DIM_Y && msg.x + BULLET_DIM_X > gamevar.frog.x && msg.x < gamevar.frog.x + FROG_DIM_X) {
                    gamevar.timer = MANCHE_LOST;
                    manche_ended = TRUE;
                    break;
                }

                // Plant bullet collision with frog bullet
                for(int i = 0; i < MAX_BULLETS_PER_FROG; i++) {
                    if(gamevar.frog_bullets[i].y >= 0 && msg.y + BULLET_DIM_Y > gamevar.frog_bullets[i].y && msg.y < gamevar.frog_bullets[i].y + BULLET_DIM_Y && msg.x + BULLET_DIM_X > gamevar.frog_bullets[i].x && msg.x < gamevar.frog_bullets[i].x + BULLET_DIM_X) {
                        // Update counter
                        gamevar.free_frog_bullet++;
                        print_free_frog_bullet(gamevar.free_frog_bullet);
                        // Free frog bullet
                        gamevar.frog_bullets[i].y = FREE_ENTITY; // Mark it as free
                        kill(process_pids.list[MIN_FROG_BULLET_ID + i], SIGKILL);
                        waitpid(process_pids.list[MIN_FROG_BULLET_ID + i], NULL, 0); // Handle died bullet process
                        process_pids.list[MIN_FROG_BULLET_ID + i] = 0;
                        // Free plant bullet
                        gamevar.plants_bullets[plant_id][entity_id].y = FREE_ENTITY; // Mark it as free
                        kill(process_pids.list[msg.id], SIGKILL);
                        waitpid(process_pids.list[msg.id], NULL, 0); // Handle died bullet process
                        process_pids.list[msg.id] = 0;
                        // De-print
                        mvwaprintw(main_scr, msg.y, msg.x, GREEN_DARKBLUE, "%*s", BULLET_DIM_X, "");
                        break;
                    }
                }
            }

            break;
        }
        
        // Refresh
        if(timestamp() - refresh_time > REFRESH_TIME_THRESHOLD) {
            wrefresh(main_scr);
            refresh_time = timestamp();
        }
    }
    wrefresh(main_scr);
    signal_all(process_pids, SIGKILL); // Killing all child processes
    for(int i = 0; i < LIM_N_ENTITIES; i++) {
        if(process_pids.list[i] != 0) {
            waitpid(process_pids.list[i], NULL, 0);
        }
    }

    // Free allocated memory
    for(int i = 0; i < N_WATER_STREAM; i++) {
        free(gamevar.croccodiles[i]);
        free(gamevar.croccodiles_kind[i]);
    }
    for(int i = 0; i < N_PLANTS; i++) {
        free(gamevar.plants_bullets[i]);
    }
    free(gamevar.croccodiles);
    free(gamevar.croccodiles_kind);
    free(gamevar.plants);
    free(gamevar.plants_sig);
    free(gamevar.frog_bullets);
    free(gamevar.plants_bullets);
    gamevar.croccodiles_kind = NULL;
    gamevar.croccodiles = gamevar.plants_bullets = NULL;
    gamevar.plants = gamevar.frog_bullets = NULL;
    gamevar.plants_sig = NULL;

    // Close file descriptors
    close(pipe_fds[PIPE_READ]);
    close(pipe_fds[PIPE_WRITE]);

    return gamevar;
}
