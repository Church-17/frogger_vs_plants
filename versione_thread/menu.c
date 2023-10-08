// Include libs
#include <curses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"

int menu(str title, List_str opts, bool nav) {
    int i, win_width = max_strlen(opts, strlen(title)) + BOX_PADW + BOX_PADE;
    WINDOW* menu_win = newwin(opts.len+BOX_PADN+BOX_PADS, win_width, (LINES - (opts.len+BOX_PADN+BOX_PADS))/2, (COLS - win_width)/2);
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

int* double_menu(str title, List_str* opts, int* setted, int opts_len, bool nav) {
    int i, win_width = strlen(title);
    for(i = 0; i < opts_len; i++) {
        win_width = max_strlen(opts[i], win_width);
    }
    win_width += win_width + LR_ARROWS + BOX_PADW + BOX_PADE;

    WINDOW* menu_win = newwin(opts_len+BOX_PADN+BOX_PADS+(2*nav), win_width, (LINES - (opts_len+BOX_PADN+BOX_PADS+2))/2, (COLS - win_width)/2);
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box the window
    wctrprintw(menu_win, 0, title); // Print title

    if(nav) {
        int key, hl = 0, old_hl = 0;
        for (i = 0; i < opts_len; i++) { // Print other option
            mvwattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, "%c", opts[i].list[0][0]);
            wprintw(menu_win, "%s", &(opts[i].list[0][1]));
            mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_DIM-strlen(opts[i].list[setted[i]]), "%s", opts[i].list[setted[i]]);
        }
        mvwattrprintw(menu_win, opts_len+BOX_PADN+1, BOX_PADW, A_UNDERLINE, "%c", 'B');
        wprintw(menu_win, "%s", "ack");
        
        while(TRUE) {
            // Update highlighted & non-highlighted option
            if(old_hl == opts_len) {
                mvwattrprintw(menu_win, old_hl+BOX_PADN+1, BOX_PADW, A_UNDERLINE, "%c", 'B');
                wprintw(menu_win, "ack ");
            } else {
                mvwattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, "%c", opts[old_hl].list[0][0]);
                wprintw(menu_win, "%s ", &(opts[old_hl].list[0][1]));
                mvwprintw(menu_win, old_hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[old_hl].list[setted[old_hl]]), "    %s", opts[old_hl].list[setted[old_hl]]);
            }
            if(hl == opts_len) {
                mvwprintw(menu_win, hl+BOX_PADN+1, BOX_PADW, " ");
                wattrprintw(menu_win, A_STANDOUT, "%s", "Back");
            } else {
                mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, " ");
                wattrprintw(menu_win, A_STANDOUT, "%s", opts[hl].list[0]);
                mvwattrprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), A_STANDOUT, "◄ %s ►", opts[hl].list[setted[hl]]);
            }

            old_hl = hl;

            key = wgetch(menu_win);
            switch (key) {
                case KEY_UP:
                case KEY_PPAGE:
                    if (--hl < 0) {
                        hl = opts_len;
                    }
                    break;
                
                case KEY_DOWN:
                case KEY_NPAGE:
                    if (++hl > opts_len) {
                        hl = 0;
                    }
                    break;

                case KEY_LEFT:
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), "%*s", (int) strlen(opts[hl].list[setted[hl]])+LR_ARROWS, "");
                    if(--setted[hl] <= 0) {
                        setted[hl] = opts[hl].len - 1;
                    }
                    break;

                case KEY_RIGHT:
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), "%*s", (int) strlen(opts[hl].list[setted[hl]])+LR_ARROWS, "");
                    if(++setted[hl] >= opts[hl].len) {
                        setted[hl] = 1;
                    }
                    break;

                case KEY_HOME:
                    hl = 0;
                    break;

                case KEY_END:
                case 'B':
                case 'b':
                    hl = opts_len;
                    break;

                case ENTER:
                    if(hl == opts_len) {
                        unwin(menu_win);
                        return setted;
                    }
                    break;

                default:
                    for(i = 0; i < opts_len; i++) {
                        if(key == opts[i].list[0][0] || key == opts[i].list[0][0]+DIFF_CAPITAL) {
                            hl = i;
                            break;
                        }
                    }
                    break;
            }
        }
    } else {
        for (i = 0; i < opts_len; i++) { // Print other option
            mvwprintw(menu_win, i+BOX_PADN, BOX_PADW, "%s", opts[i].list[0]);
            mvwprintw(menu_win, i+BOX_PADN, win_width+BOX_PADE-strlen(opts[i].list[setted[i]]), "%s", opts[i].list[setted[i]]);
        }
        wgetch(menu_win);
        unwin(menu_win);
        return NULL;
    }
}

// Home Menu function
void home_menu(void) {
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
                settings();
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
void settings() {
    str set1[4] = {"Difficulty", "Easy", "Normal", "Difficult"};
    str set2[4] = {"Skin", "Common", "Rare", "Epic"};
    List_str opts[2];
    opts[0].list = set1;
    opts[0].len = 4;
    opts[1].list = set2;
    opts[1].len = 4;
    int setted[2] = {2, 3};
    double_menu(" Settings ", opts, setted, 2, TRUE);
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
