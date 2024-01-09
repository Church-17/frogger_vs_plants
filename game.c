// Include libs
#include <unistd.h>
#include "main.h"
#include "menu.h"
#include "manche.h"
#include "music.h"
#include "utils.h"
#include "struct.h"

// Function prototypes
int play_game(void);

// Start demo in main_scr
void demo(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    print_background(holes_occupied);

    // Figlet
    wattron(main_scr, FIGLET_COLOR);
    mvwprintw(main_scr, LINE_FIGLET + 0, TITLE_FIGLET_X, "███████╗██████╗  ██████╗  ██████╗  ██████╗ ███████╗██████╗     ██╗   ██╗███████╗    ██████╗ ██╗      █████╗ ███╗   ██╗████████╗███████╗");
    mvwprintw(main_scr, LINE_FIGLET + 1, TITLE_FIGLET_X, "██╔════╝██╔══██╗██╔═══██╗██╔════╝ ██╔════╝ ██╔════╝██╔══██╗    ██║   ██║██╔════╝    ██╔══██╗██║     ██╔══██╗████╗  ██║╚══██╔══╝██╔════╝");
    mvwprintw(main_scr, LINE_FIGLET + 2, TITLE_FIGLET_X, "█████╗  ██████╔╝██║   ██║██║  ███╗██║  ███╗█████╗  ██████╔╝    ██║   ██║███████╗    ██████╔╝██║     ███████║██╔██╗ ██║   ██║   ███████╗");
    mvwprintw(main_scr, LINE_FIGLET + 3, TITLE_FIGLET_X, "██╔══╝  ██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  ██╔══██╗    ╚██╗ ██╔╝╚════██║    ██╔═══╝ ██║     ██╔══██║██║╚██╗██║   ██║   ╚════██║");
    mvwprintw(main_scr, LINE_FIGLET + 4, TITLE_FIGLET_X, "██║     ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗██║  ██║     ╚████╔╝ ███████║    ██║     ███████╗██║  ██║██║ ╚████║   ██║   ███████║");
    mvwprintw(main_scr, LINE_FIGLET + 5, TITLE_FIGLET_X, "╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝      ╚═══╝  ╚══════╝    ╚═╝     ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝");
    wattroff(main_scr, FIGLET_COLOR);
}

// Manage more games
bool game() {
    // Init vars
    bool do_play = TRUE, do_quit = FALSE; // Flag
    int game_ret;

    // Loop for retrying play
    while(do_play && !do_quit) {
        game_ret = play_game();
        switch(game_ret) {
            case OVER_RETR_ID: // Play
                break;

            case OVER_BACK_ID: // Back to menu
                do_play = FALSE;
                break;

            case OVER_QUIT_ID:
                do_quit = TRUE;
                break;
        }
    }
    wclear(main_scr);
    return do_quit;
}

// Play a game handling more manche, return gameover_menu index
int play_game(void) {
    // Init vars
    bool holes_occupied[N_HOLES] = {FALSE};
    int tmp_score, usleep_ret;
    Game_t gamevar;
    gamevar.score = 0;
    gamevar.lifes = N_LIFES;

    // Loop for play n manche saving the remained time and updating lifes
    for(int i = 0; i < N_MANCHES && gamevar.lifes; i++) {
        gamevar = play_manche(gamevar.score, gamevar.lifes, holes_occupied);
        switch(gamevar.timer) {
            case MANCHE_LOST:
                gamevar.lifes--;
                i--;
                break;

            case MANCHE_RETR:
                return OVER_RETR_ID;
                break;

            case MANCHE_CLOSE:
                return OVER_BACK_ID;
                break;

            case MANCHE_QUIT:
                return OVER_QUIT_ID;
                break;
        }

        // Update score
        tmp_score = gamevar.score;
        gamevar.score += gamevar.timer * SCORE_MULTIPLIER;

        // Animation of time & score (if usleep is interrupted, interrupt animation & pause game)
        for(usleep_ret = usleep(MSEC_IN_SEC * 300); usleep_ret == 0 && gamevar.timer > 0; usleep_ret = usleep(MSEC_IN_SEC * 50)) {
            gamevar.timer--;
            tmp_score += SCORE_MULTIPLIER;
            print_time(gamevar.timer);
            print_score(tmp_score);
            wrefresh(main_scr);
        }
        if(usleep_ret == 0) {
            usleep(MSEC_IN_SEC * 500);
        }
        flushinp(); // Ignore any user input received during animation
    }

    // Update score & bullet
    gamevar.score *= gamevar.lifes;
    gamevar.free_frog_bullet = MAX_BULLETS_PER_FROG;

    // Animation of score & lifes
    print_lifes(0);
    wrefresh(main_scr);
    usleep(MSEC_IN_SEC * 300);
    flushinp(); // Ignore any user input received during animation

    // If score was one of the best, add it to the best scores
    if(gamevar.score > 0) {
        int index_new_score;
        Dict_str_int best = rd_best(); // Read actual best scores
        for(index_new_score = best.len; index_new_score > 0 && gamevar.score > best.val[index_new_score-1]; index_new_score--) { // Check if score is gtr than the least best score
            if(index_new_score < N_BEST) { // If it is, pass the previous best score down (if it is possible)
                best.key[index_new_score] = best.key[index_new_score-1];
                best.val[index_new_score] = best.val[index_new_score-1];
            }
        }
        if(index_new_score < N_BEST) { // If the new score is a best score, write in best scores
            gamevar.win = HIGH_SCORE_GAME;
            best.key[index_new_score] = getenv("USER");
            best.val[index_new_score] = gamevar.score;
            if(best.len < N_BEST) { // Increment best size if needed
                best.len++;
            }
            wr_best(best);
        } else {
            gamevar.win = WIN_GAME;
        }
    } else {
        gamevar.win = LOST_GAME;
    }
    print_game(&gamevar);
    wrefresh(main_scr);

    return gameover_menu(gamevar.score, &gamevar);
}

void print_game(const Game_t* gamevar) {
    print_background(gamevar->holes_occupied); // Print background
    print_free_frog_bullet(gamevar->free_frog_bullet);
    print_time(gamevar->timer); // Print time
    print_score(gamevar->score); // Print score
    print_lifes(gamevar->lifes); // Print lifes

    if(gamevar->croccodiles != NULL) {
        // Print croccodiles
        for(int i = 0; i < N_WATER_STREAM; i++) {
            for(int j = 0; j < MAX_CROCCODILE_PER_STREAM; j++) {
                if(gamevar->croccodiles[i][j].y >= 0) {
                    print_croccodile(gamevar->croccodiles[i][j], gamevar->stream_speed[i], gamevar->croccodiles_kind[i][j]);
                }
            }
        }
        print_frog(gamevar); // Print frog
        // Print plants
        for(int i = 0; i < N_PLANTS; i++) {
            if(gamevar->plants[i].y >= 0) {
                print_plant(gamevar->plants[i]);
            }
            // Print plants bullets
            for(int j = 0; j < MAX_BULLETS_PER_PLANT; j++) {
                if(gamevar->plants_bullets[i][j].y >= 0) {
                    print_bullet(gamevar->plants_bullets[i][j]);
                }
            }
        }
        // Print frog bullets
        for(int i = 0; i < MAX_BULLETS_PER_FROG; i++) {
            if(gamevar->frog_bullets[i].y >= 0) {
                print_bullet(gamevar->frog_bullets[i]);
            }
        }
    }

    print_figlet(gamevar->win); // Print figlet if needed
}
