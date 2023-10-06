// Include libs
#include <curses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"

int menu(str title, List_str opts, bool nav) {
    int i, max_optlen = max_strlen(opts, strlen(title));
    WINDOW* menu_win = newwin(opts.len + BOX_PADN + BOX_PADS, max_optlen + BOX_PADE + BOX_PADW, (LINES - (opts.len + BOX_PADN + BOX_PADS))/2, (COLS - (max_optlen + BOX_PADE + BOX_PADW))/2);
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box the window
    wctrprintw(menu_win, 0, title); // Print title

    if(nav) {
        int key, hl = 0, old_hl = 0;
        for (i = 0; i < opts.len; i++) { // Print other option
            mvwattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, "%c", opts.list[i][0]);
            wprintw(menu_win, "%s", &(opts.list[i][1]));
        }
        while(TRUE) {
            // Update highlighted & non-highlighted option
            mvwattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, "%c", opts.list[old_hl][0]);
            wprintw(menu_win, "%s ", &(opts.list[old_hl][1]));
            mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, " ");
            wattrprintw(menu_win, A_STANDOUT, "%s", opts.list[hl]);

            old_hl = hl;

            key = wgetch(menu_win);
            switch (key) {
                case KEY_UP:
                case KEY_LEFT:
                case KEY_PPAGE:
                    if (--hl == -1) {
                        hl = opts.len - 1;
                    }
                    break;
                
                case KEY_DOWN:
                case KEY_RIGHT:
                case KEY_NPAGE:
                    if (++hl == opts.len) {
                        hl = 0;
                    }
                    break;

                case KEY_HOME:
                    hl = 0;
                    break;

                case KEY_END:
                    hl = opts.len-1;
                    break;

                case ENTER:
                    unwin(menu_win);
                    return hl;

                default:
                    for(i = 0; i < opts.len; i++) {
                        if(key == opts.list[i][0] || key == opts.list[i][0]+32) {
                            hl = i;
                            break;
                        }
                    }
                    break;
            }
        }
    } else {
        for (i = 0; i < opts.len; i++) { // Print other option
            mvwprintw(menu_win, i+BOX_PADN, BOX_PADW, "%s", opts.list[i]);
        }
        wgetch(menu_win);
        unwin(menu_win);
        return 0;
    }
}

// Home Menu function
void home_menu() {
    int chosen;
    str list[N_OPTS_HOME] = {"New game", "Best scores", "Settings", "Credits", "Quit"};
    List_str opts;
    opts.list = list;
    opts.len = N_OPTS_HOME;
    while(TRUE) {
        chosen = menu(TITLE, opts, TRUE);
        switch(chosen) {
            case 0:
                game();
                break;
            case 1:
                best_scores();
                break;
            case 2:
                game_settings();
                break;
            case 3:
                credits_menu();
                break;
            default:
                return;
        }
    }
}

// Best scores screen
void best_scores() {
    
}

// Game settings menu
void game_settings() {

}

// Credits screen
void credits_menu() {
    str list[N_OPTS_CREDS] = {"SOPR Project 23-24:", "", "Francesco Cardia", "Matteo Chiesa"};
    List_str opts;
    opts.list = list;
    opts.len = N_OPTS_CREDS;
    menu(" Credits ", opts, FALSE);
}

int pause_menu() {
    str list[N_OPTS_PAUSE] = {"Resume", "New game", "Settings", "Home menu", "Quit"};
    List_str opts;
    opts.list = list;
    opts.len = N_OPTS_PAUSE;
    int chosen = menu(" Pause ", opts, TRUE);
    switch(chosen) {
        default:
            break;
    }
    return 0;
}

void endgame_menu(int score) {
    
}
