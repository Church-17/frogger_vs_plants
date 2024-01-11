// Include libs
#include <unistd.h>
#include "../main.h"
#include "../menu.h"
#include "../manche.h"
#include "../music.h"
#include "../utils.h"
#include "../struct.h"
#include "thread.h"
#include "entity.h"

// Define constant
#define RESIZE_TIME_THRESHOLD 100
#define REFRESH_TIME_THRESHOLD 100

// Play a manche, return game vars with in gamevar.timer the time remaining or a manche_id
Game_t play_manche(int score, int n_lifes, bool* holes_occupied) {
    // Init vars
    bool occupied_plant;
    pthread_t array_tids[LIM_N_ENTITIES] = {0}; // ID for every thread
    List_thread thread_tids = {array_tids, LIM_N_ENTITIES};
    int thread_params[LIM_N_ENTITIES][N_ENTITY_PARAMS]; // Matrix to store params for each entity; each line will be passed to thread as params
    int stream_speed[N_WATER_STREAM];
    int plants_x[N_PLANTS];
    int id;

    init_semaphore(); // Semaphores
    // Reset var
    for(int i = 0; i < MIN_PLANT_ID - MIN_CROCCODILE_ID; i++) {
        frog_on_croccodile[i] = croccodile_shotted[i] = FALSE;
    }

    // Thread
    async_exec(&thread_tids, FROG_ID, &frog_thread, NULL); // Create thread for frog handling the errors
    async_exec(&thread_tids, TIME_ID, &time_thread, NULL); // Create thread for time handling the errors
    for(int i = 0; i < N_WATER_STREAM; i++) {
        // Randomize speed & direction of each stream
        stream_speed[i] = rand_range(MIN_STREAM_SPEED, MAX_STREAM_SPEED) * (rand_range(0, 2) ? 1 : -1);
        // Write croccodile params
        id = i*MAX_CROCCODILE_PER_STREAM + MIN_CROCCODILE_ID;
        thread_params[id][CROCCODILE_ID_INDEX] = id;
        thread_params[id][CROCCODILE_STREAM_INDEX] = i;
        thread_params[id][CROCCODILE_SPEED_INDEX] = stream_speed[i];
        // Create thread for croccodile handling the errors
        async_exec(&thread_tids, id, &croccodile_thread, thread_params[id]);
    }
    for(int i = 0; i < N_PLANTS; i++) {
        id = MIN_PLANT_ID + i;
        thread_params[id][PLANT_ID_INDEX] = id;
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
        thread_params[id][PLANT_X_INDEX] = plants_x[i];
        async_exec(&thread_tids, id, &plant_thread, thread_params[id]);
    }
    
    // --- PARENT PROCESS ---

    // Init control vars
    bool manche_ended = FALSE; // Flag
    int rd_index = 0;
    int entity_stream, entity_id, next_croccodile_id, restore_croccodile_x, restore_croccodile_len; // Helper vars for croccodile
    int stream_last[N_WATER_STREAM] = {0}; // Track which croccodile was the last of each stream
    int next_frog_bullet = 0, next_plant_bullet[N_PLANTS] = {0}, plant_id;
    time_t resize_time = 0, refresh_time = 0; // Var to store time of the last continue to prevent multiple resize message at once
    Message msg; // Define msg to store buffer message

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
        
        // Read from buffer
        msg = read_msg(&rd_index);

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
                pause_manche(); // Pausing all threads
                if(msg.id == CLOSE_ID) {
                    entity_id = quit_menu(&gamevar);    
                    switch(entity_id) {
                        case NO_ID:
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
                resume_manche(); // Resume all threads
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
                        gamevar.frog_bullets[next_frog_bullet].y = INCOMING_ENTITY;
                        // Calc bullet params
                        id = next_frog_bullet + MIN_FROG_BULLET_ID;
                        thread_params[id][BULLET_ID_INDEX] = next_frog_bullet + MIN_FROG_BULLET_ID;
                        thread_params[id][BULLET_Y_INDEX] = gamevar.frog.y - 1;
                        thread_params[id][BULLET_X_INDEX] = gamevar.frog.x + FROG_DIM_X/2;
                        // Thread
                        async_exec(&thread_tids, id, &bullet_thread, thread_params[id]);
                        // Update next bullet & counter
                        next_frog_bullet = mod(next_frog_bullet + 1, MAX_BULLETS_PER_FROG);
                        gamevar.free_frog_bullet--;
                        print_free_frog_bullet(gamevar.free_frog_bullet);
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
                                frog_on_croccodile[entity_stream*MAX_CROCCODILE_PER_STREAM + i] = TRUE;
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
                    thread_tids.list[msg.id] = 0;
                    frog_on_croccodile[entity_stream*MAX_CROCCODILE_PER_STREAM + entity_id] = FALSE;
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
                        pthread_cancel(thread_tids.list[MIN_FROG_BULLET_ID + i]); // Kill bullet thread
                        thread_tids.list[MIN_FROG_BULLET_ID + i] = 0;
                        if(msg.sig != CROCCODILE_GOOD_SIG) { // Change kindness of croccodile if it is bad
                            change_croccodile_shotted(msg.id, TRUE);
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
                        thread_params[id][CROCCODILE_ID_INDEX] = id;
                        thread_params[id][CROCCODILE_STREAM_INDEX] = entity_stream;
                        thread_params[id][CROCCODILE_SPEED_INDEX] = stream_speed[entity_stream];
                        async_exec(&thread_tids, id, &croccodile_thread, thread_params[id]); // Create thread for croccodile handling the errors
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
                        pthread_cancel(thread_tids.list[msg.id]);
                        gamevar.plants[entity_id].y = INCOMING_ENTITY;
                        thread_params[msg.id][PLANT_ID_INDEX] = msg.id;
                        do {
                            occupied_plant = FALSE;
                            thread_params[msg.id][PLANT_X_INDEX] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
                            for(int j = 0; j < N_PLANTS; j++) {
                                if(j != entity_id && thread_params[msg.id][PLANT_X_INDEX] + PLANT_DIM_X > gamevar.plants[j].x && thread_params[msg.id][PLANT_X_INDEX] < gamevar.plants[j].x + PLANT_DIM_X) {
                                    occupied_plant = TRUE;
                                    break;
                                }
                            }
                        } while(occupied_plant);
                        async_exec(&thread_tids, msg.id, &plant_thread, thread_params[msg.id]);
                        break;
                    }
                    // Update coordinates & print
                    gamevar.plants[entity_id].y = msg.y;
                    gamevar.plants[entity_id].x = msg.x;
                    print_plant(gamevar.plants[entity_id], msg.sig);
                } else if(msg.sig == PLANT_SHOT_SIG) { // Shot
                    if(gamevar.plants_bullets[entity_id][next_plant_bullet[entity_id]].y == FREE_ENTITY) {
                        gamevar.plants_bullets[entity_id][next_plant_bullet[entity_id]].y = INCOMING_ENTITY;
                        // Calc bullet params
                        id = next_plant_bullet[entity_id] + MAX_BULLETS_PER_PLANT*entity_id + MIN_PLANT_BULLET_ID;
                        thread_params[id][BULLET_ID_INDEX] = id;
                        thread_params[id][BULLET_Y_INDEX] = gamevar.plants[entity_id].y + PLANT_DIM_Y;
                        thread_params[id][BULLET_X_INDEX] = gamevar.plants[entity_id].x + PLANT_DIM_X/2;
                        async_exec(&thread_tids, id, &bullet_thread, thread_params[id]); // Create thread
                        next_plant_bullet[entity_id] = mod(next_plant_bullet[entity_id] + 1, MAX_BULLETS_PER_PLANT); // Update next
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
                }

                // If frog bullet is out of limit free it
                if(msg.y < LINE_BANK_1) {
                    gamevar.free_frog_bullet++;
                    print_free_frog_bullet(gamevar.free_frog_bullet);
                    gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                    thread_tids.list[msg.id] = 0;
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
                            pthread_cancel(thread_tids.list[msg.id]);
                            thread_tids.list[msg.id] = 0;
                            if(gamevar.croccodiles_kind[entity_stream][i] != CROCCODILE_GOOD_SIG) { // Change kindness of croccodile if it is bad
                                change_croccodile_shotted(MIN_CROCCODILE_ID + MAX_CROCCODILE_PER_STREAM*entity_stream + i, TRUE);
                            }
                            break;
                        }
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
                                pthread_cancel(thread_tids.list[msg.id]);
                                thread_tids.list[msg.id] = 0;
                                // Free plant bullet
                                gamevar.plants_bullets[i][j].y = FREE_ENTITY; // Mark it as free
                                pthread_cancel(thread_tids.list[MIN_PLANT_BULLET_ID + MAX_BULLETS_PER_PLANT*i + j]);
                                thread_tids.list[MIN_PLANT_BULLET_ID + MAX_BULLETS_PER_PLANT*i + j] = 0;
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
                            // Update counter
                            gamevar.free_frog_bullet++;
                            print_free_frog_bullet(gamevar.free_frog_bullet);
                            // Free frog bullet
                            gamevar.frog_bullets[entity_id].y = FREE_ENTITY; // Mark it as free
                            pthread_cancel(thread_tids.list[msg.id]);
                            thread_tids.list[msg.id] = 0;
                            // De-print
                            for(int j = 0; j < PLANT_DIM_Y; j++) {
                                mvwaprintw(main_scr, gamevar.plants[i].y + j, gamevar.plants[i].x, GREEN_PURPLE, "%*s", PLANT_DIM_X, "");
                            }
                            // Kill plant & spawn another
                            id = MIN_PLANT_ID + i;
                            pthread_cancel(thread_tids.list[id]);
                            gamevar.plants[i].y = INCOMING_ENTITY;
                            thread_params[id][PLANT_ID_INDEX] = id;
                            do {
                                occupied_plant = FALSE;
                                thread_params[id][PLANT_X_INDEX] = rand_range(0, MAIN_COLS - PLANT_DIM_X);
                                for(int j = 0; j < N_PLANTS; j++) {
                                    if(j != i && thread_params[id][PLANT_X_INDEX] + PLANT_DIM_X > gamevar.plants[j].x && thread_params[id][PLANT_X_INDEX] < gamevar.plants[j].x + PLANT_DIM_X) {
                                        occupied_plant = TRUE;
                                        break;
                                    }
                                }
                            } while(occupied_plant);
                            async_exec(&thread_tids, id, &plant_thread, thread_params[id]);
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
                }

                // If frog bullet is out of screen free it
                if(msg.y >= MAIN_ROWS) {
                    gamevar.plants_bullets[plant_id][entity_id].y = FREE_ENTITY; // Mark it as free
                    thread_tids.list[msg.id] = 0;
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
                        pthread_cancel(thread_tids.list[MIN_FROG_BULLET_ID + i]);
                        thread_tids.list[MIN_FROG_BULLET_ID + i] = 0;
                        // Free plant bullet
                        gamevar.plants_bullets[plant_id][entity_id].y = FREE_ENTITY; // Mark it as free
                        pthread_cancel(thread_tids.list[msg.id]);
                        thread_tids.list[msg.id] = 0;
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
        }
    }
    wrefresh(main_scr);
    quit_manche(thread_tids); // Killing all threads

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

    destroy_semaphore(); // Destroy semaphore

    return gamevar;
}
