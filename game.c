// Include libs
#include "main.h"
#include "menu.h"
#include "manche.h"
#include "utils.h"
#include "struct.h"

// Function prototypes
int play_game(void);

// Start demo in main_scr
void demo(void) {
    bool holes_occupied[N_HOLES] = {FALSE};
    print_background(holes_occupied);

    // Figlet
    wattron(main_scr, FIGLET_COL);
    mvwprintw(main_scr, LINE_FIGLET + 0, 7, "███████╗██████╗  ██████╗  ██████╗  ██████╗ ███████╗██████╗     ██╗   ██╗███████╗    ██████╗ ██╗      █████╗ ███╗   ██╗████████╗███████╗");
    mvwprintw(main_scr, LINE_FIGLET + 1, 7, "██╔════╝██╔══██╗██╔═══██╗██╔════╝ ██╔════╝ ██╔════╝██╔══██╗    ██║   ██║██╔════╝    ██╔══██╗██║     ██╔══██╗████╗  ██║╚══██╔══╝██╔════╝");
    mvwprintw(main_scr, LINE_FIGLET + 2, 7, "█████╗  ██████╔╝██║   ██║██║  ███╗██║  ███╗█████╗  ██████╔╝    ██║   ██║███████╗    ██████╔╝██║     ███████║██╔██╗ ██║   ██║   ███████╗");
    mvwprintw(main_scr, LINE_FIGLET + 3, 7, "██╔══╝  ██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  ██╔══██╗    ╚██╗ ██╔╝╚════██║    ██╔═══╝ ██║     ██╔══██║██║╚██╗██║   ██║   ╚════██║");
    mvwprintw(main_scr, LINE_FIGLET + 4, 7, "██║     ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗██║  ██║     ╚████╔╝ ███████║    ██║     ███████╗██║  ██║██║ ╚████║   ██║   ███████║");
    mvwprintw(main_scr, LINE_FIGLET + 5, 7, "╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝      ╚═══╝  ╚══════╝    ╚═╝     ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝");
    wattroff(main_scr, FIGLET_COL);
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
    bool holes_occupied[N_HOLES] = {FALSE};
    int i, score = 0, n_lifes = N_LIFES;
    Game_t gamevar;

    // Loop for play n manche saving the remained time and updating lifes
    for(i = 0; i < N_HOLES && n_lifes; i++) {
        gamevar = play_manche(score, n_lifes, holes_occupied);
        switch(gamevar.timer) {
            case MANCHE_LOST:
                gamevar.lifes--;
                if(gamevar.lifes > 0) {
                    i--;
                } else {
                    print_lifes(gamevar.lifes);
                }
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

            default:
                score += gamevar.timer * (DIFF_SET + 1);
                break;
        }
        n_lifes = gamevar.lifes;
    }

    // Update score
    gamevar.score = score * n_lifes;
    print_score(gamevar.score);

    // If score was one of the best, add it to the best scores
    if(gamevar.score > 0) {
        int i; // Index where write the new best score (if i == N_BEST, it isn't a new best score)
        Dict_str_int best = rd_best(); // Read actual best scores
        for(i = best.len; i > 0 && gamevar.score > best.val[i-1]; i--) { // Check if score is gtr than the least best score
            if(i < N_BEST) { // If it is, pass the previous best score down (if it is possible)
                best.key[i] = best.key[i-1];
                best.val[i] = best.val[i-1];
            }
        }
        if(i < N_BEST) { // If the new score is a best score, write in best scores
            gamevar.win = HIGH_SCORE_GAME;
            best.key[i] = getenv("USER");
            best.val[i] = gamevar.score;
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
    print_background(gamevar.holes_occupied);
    print_figlet(gamevar.win);
    wrefresh(main_scr);

    return gameover_menu(gamevar.score, &gamevar);
}

void print_game(const Game_t* gamevar) {
    int i, j;

    // Print background
    print_background(gamevar->holes_occupied);

    // Print time
    print_time(gamevar->timer);

    // Print score
    print_score(gamevar->score);

    // Print lifes
    print_lifes(gamevar->lifes);

    if(gamevar->croccodiles != NULL) {
        // Print croccodiles
        for(i = 0; i < N_WATER_STREAM; i++) {
            for(j = 0; j < MAX_CROCCODILE_PER_STREAM; j++) {
                if(gamevar->croccodiles[i][j].y >= 0) {
                    print_croccodile(gamevar->croccodiles[i][j], gamevar->stream_speed > 0, gamevar->bad_croccodiles[i][j]);
                }
            }
        }

        // Print frog
        print_frog(gamevar);
    }
    

    // Print figlet if needed
    print_figlet(gamevar->win);
}
