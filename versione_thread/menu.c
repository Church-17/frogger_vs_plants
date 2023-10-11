// Include libs
#include <curses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "strings.h"

int menu(str title, List_str sets, bool nav) {
    int i, win_width = max_strlen(sets, strlen(title)) + BOX_PADW + BOX_PADE; // Calc window width
    WINDOW* menu_win = newctrwin(sets.len+BOX_PADN+BOX_PADS, win_width); // Centered win
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box win
    wctrprintw(menu_win, 0, title); // Print title

    if(nav) {
        int key, hl = 0, old_hl = 0; // Init vars

        // Print options with first letter underlined
        for (i = 0; i < sets.len; i++) {
            mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, sets.list[i]);
        }
        while(TRUE) {
            // Update highlighted & non-highlighted option
            mvwfattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, sets.list[old_hl]);
            wprintw(menu_win, "%*s", PAD_HL, "");
            mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", PAD_HL, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", sets.list[hl]);

            old_hl = hl; // Track old hl

            key = wgetch(menu_win);
            switch (key) {
                case KEY_UP:
                case KEY_LEFT:
                case KEY_PPAGE:
                    if (--hl < 0) {
                        hl = sets.len - 1;
                    }
                    break;
                
                case KEY_DOWN:
                case KEY_RIGHT:
                case KEY_NPAGE:
                    if (++hl >= sets.len) {
                        hl = 0;
                    }
                    break;

                case KEY_HOME:
                    hl = 0;
                    break;

                case KEY_END:
                    hl = sets.len-1;
                    break;

                case ENTER:
                    unwin(menu_win);
                    return hl;

                default:
                    // Check first letters
                    for(i = 0; i < sets.len; i++) {
                        if(key == sets.list[i][0] || key == sets.list[i][0]+DIFF_CAPITAL) {
                            hl = i;
                            break;
                        }
                    }
                    break;
            }
        }
    } else {
        // Prints options
        for (i = 0; i < sets.len; i++) {
            mvwprintw(menu_win, i+BOX_PADN, BOX_PADW, "%s", sets.list[i]);
        }
        wgetch(menu_win);
        unwin(menu_win);
        return 0;
    }
}

int* double_menu(str title, List_str sets, List_str* opts, int* setted, bool nav) {
    int i, win_width = max_strlen(sets, strlen(title));
    // Calc window width
    for(i = 0; i < sets.len; i++) {
        win_width = max_strlen(opts[i], win_width);
    }
    win_width += win_width + LR_ARROWS + BOX_PADW + BOX_PADE;

    WINDOW* menu_win = newctrwin(sets.len+BOX_PADN+BOX_PADS+(2*nav), win_width); // 2*nav create space for last button if nav
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box the window
    wctrprintw(menu_win, 0, title); // Print title

    if(nav) {
        int key, hl = 0, old_hl = 0;
        str last = "Back";
        // Print sets and opts
        for (i = 0; i < sets.len; i++) {
            mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, sets.list[i]);
            mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_DIM-strlen(opts[i].list[setted[i]]), "%s", opts[i].list[setted[i]]);
        }
        // Print last button
        mvwfattrprintw(menu_win, sets.len+BOX_PADN+1, BOX_PADW, A_UNDERLINE, last);
        
        while(TRUE) {
            // Update highlighted & non-highlighted option
            if(old_hl == sets.len) {
                mvwfattrprintw(menu_win, old_hl+BOX_PADN+1, BOX_PADW, A_UNDERLINE, last);
                wprintw(menu_win, "%*s", PAD_HL, "");
            } else {
                mvwfattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, sets.list[old_hl]);
                wprintw(menu_win, "%*s", PAD_HL, "");
                mvwprintw(menu_win, old_hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[old_hl].list[setted[old_hl]]), "    %s", opts[old_hl].list[setted[old_hl]]);
            }
            if(hl == sets.len) {
                mvwprintw(menu_win, hl+BOX_PADN+1, BOX_PADW, "%*s", PAD_HL, "");
                wattrprintw(menu_win, A_STANDOUT, "%s", last);
            } else {
                mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", PAD_HL, "");
                wattrprintw(menu_win, A_STANDOUT, "%s", sets.list[hl]);
                mvwattrprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), A_STANDOUT, "◄ %s ►", opts[hl].list[setted[hl]]);
            }

            old_hl = hl;

            key = wgetch(menu_win);
            switch (key) {
                case KEY_UP:
                case KEY_PPAGE:
                    if (--hl < 0) {
                        hl = sets.len;
                    }
                    break;
                
                case KEY_DOWN:
                case KEY_NPAGE:
                    if (++hl > sets.len) {
                        hl = 0;
                    }
                    break;

                case KEY_LEFT:
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), "%*s", (int) strlen(opts[hl].list[setted[hl]])+LR_ARROWS, "");
                    if(--setted[hl] < 0) {
                        setted[hl] = opts[hl].len - 1;
                    }
                    break;

                case KEY_RIGHT:
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), "%*s", (int) strlen(opts[hl].list[setted[hl]])+LR_ARROWS, "");
                    if(++setted[hl] >= opts[hl].len) {
                        setted[hl] = 0;
                    }
                    break;

                case KEY_HOME:
                    hl = 0;
                    break;

                case KEY_END:
                    hl = sets.len;
                    break;

                case ENTER:
                    if(hl == sets.len) {
                        unwin(menu_win);
                        return setted;
                    }
                    break;

                default:
                    // Check first letters
                    for(i = 0; i < sets.len; i++) {
                        if(key == sets.list[i][0] || key == sets.list[i][0]+DIFF_CAPITAL) {
                            hl = i;
                            break;
                        }
                    }
                    if(key == last[0] || key == last[0]+DIFF_CAPITAL) {
                        hl = sets.len;
                    }
                    break;
            }
        }
    } else {
        // Print sets & opts
        for (i = 0; i < sets.len; i++) {
            mvwprintw(menu_win, i+BOX_PADN, BOX_PADW, "%s", sets.list[i]);
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
    List_str sets;
    sets.list = list;
    sets.len = N_OPTS_HOME;
    while(TRUE) {
        chosen = menu(TITLE, sets, TRUE);
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
    str set[2] = {"Difficulty", "Skin"};
    List_str sets;
    sets.list = set;
    sets.len = 2;
    str opt1[3] = {"Easy", "Normal", "Difficult"};
    str opt2[3] = {"Common", "Rare", "Epic"};
    List_str opts[2];
    opts[0].list = opt1;
    opts[0].len = 3;
    opts[1].list = opt2;
    opts[1].len = 3;
    int setted[2] = {1, 2};
    double_menu(" Settings ", sets, opts, setted, TRUE);
}

// Credits screen
void credits_menu() {
    str list[N_OPTS_CREDS] = {"SOPR Project 23-24:", "", "Francesco Cardia", "Matteo Chiesa"};
    List_str sets;
    sets.list = list;
    sets.len = N_OPTS_CREDS;
    menu(" Credits ", sets, FALSE);
}

int pause_menu() {
    str list[N_OPTS_PAUSE] = {"Resume", "New game", "Settings", "Home menu", "Quit"};
    List_str sets;
    sets.list = list;
    sets.len = N_OPTS_PAUSE;
    int chosen = menu(" Pause ", sets, TRUE);
    switch(chosen) {
        default:
            break;
    }
    return 0;
}

void endgame_menu(int score) {
    
}
