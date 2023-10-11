// Include libs
#include <curses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "strings.h"

void view(str title, List_str sx, List_str dx) {
    if(sx.len != dx.len) {
        return;
    }
    int i, win_width = max_strlen(sx, strlen(title)) + max_strlen(dx, 0) + BOX_PADW + BOX_PADE; // Calc window width
    WINDOW* menu_win = newctrwin(sx.len+BOX_PADN+BOX_PADS, win_width); // Centered win
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box win
    wctrprintw(menu_win, 0, title); // Print title

    // Prints options
    for(i = 0; i < sx.len; i++) {
        mvwprintw(menu_win, i+BOX_PADN, BOX_PADW, "%s", sx.list[i]);
        mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_DIM-strlen(dx.list[i]), "%s", dx.list[i]);
    }
    wgetch(menu_win);
    unwin(menu_win);
}

str menu(str title, List_str set) {
    int i, key, hl = 0, old_hl = 0, win_width = max_strlen(set, strlen(title)) + BOX_PADW + BOX_PADE; // Calc window width
    WINDOW* menu_win = newctrwin(set.len+BOX_PADN+BOX_PADS, win_width); // Centered win
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box win
    wctrprintw(menu_win, 0, title); // Print title

    // Print options with first letter underlined
    for(i = 0; i < set.len; i++) {
        mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[i]);
    }
    while(TRUE) {
        // Update highlighted & non-highlighted option
        mvwfattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[old_hl]);
        wprintw(menu_win, "%*s", PAD_HL, "");
        mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", PAD_HL, "");
        wattrprintw(menu_win, A_STANDOUT, "%s", set.list[hl]);

        old_hl = hl; // Track old hl

        key = wgetch(menu_win);
        switch (key) {
            case KEY_UP:
            case KEY_LEFT:
            case KEY_PPAGE:
                if (--hl < 0) {
                    hl = set.len - 1;
                }
                break;
            
            case KEY_DOWN:
            case KEY_RIGHT:
            case KEY_NPAGE:
                if (++hl >= set.len) {
                    hl = 0;
                }
                break;

            case KEY_HOME:
                hl = 0;
                break;

            case KEY_END:
                hl = set.len-1;
                break;

            case ENTER:
                unwin(menu_win);
                return set.list[hl];

            default:
                // Check first letters
                for(i = 0; i < set.len; i++) {
                    if(key == set.list[i][0] || key == set.list[i][0]+DIFF_CAPITAL) {
                        hl = i;
                        break;
                    }
                }
                break;
        }
    }
}

void settings(void) {
    int i, set_width, opts_width = 0, win_width, key, hl = 0, old_hl = 0;
    int setted[2] = {1, 2}; // PREDEFINED SETTINGS
    str list[] = SETTINGS_SET;
    str opt1[] = DIFFICULTY_SET;
    str opt2[] = SKIN_SET;

    List_str set;
    set.list = list;
    set.len = N_SETTINGS_SET;
    List_str opts[N_SETTINGS_SET];
    opts[0].list = opt1;
    opts[0].len = N_DIFFICULTY_SET;
    opts[1].list = opt2;
    opts[1].len = N_SKIN_SET;

    // Calc window width
    set_width = max_strlen(set, strlen(SETTINGS));
    for(i = 0; i < set.len; i++) {
        opts_width = max_strlen(opts[i], opts_width);
    }
    win_width = set_width + opts_width + LR_ARROWS + BOX_PADW + BOX_PADE;

    WINDOW* menu_win = newctrwin(set.len+BOX_PADN+BOX_PADS+2, win_width);
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box the window
    wctrprintw(menu_win, 0, SETTINGS); // Print title

    // Print set and opts
    for(i = 0; i < set.len; i++) {
        mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[i]);
        mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_DIM-strlen(opts[i].list[setted[i]]), "%s", opts[i].list[setted[i]]);
    }
    // Print last buttons
    str last = BACK;
    mvwfattrprintw(menu_win, set.len+BOX_PADN+1, BOX_PADW, A_UNDERLINE, last);
    
    while(TRUE) {
        // Update highlighted & non-highlighted option
        if(old_hl == set.len) {
            mvwfattrprintw(menu_win, old_hl+BOX_PADN+1, BOX_PADW, A_UNDERLINE, last);
            wprintw(menu_win, "%*s", PAD_HL, "");
        } else {
            mvwfattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[old_hl]);
            wprintw(menu_win, "%*s", PAD_HL, "");
            mvwprintw(menu_win, old_hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[old_hl].list[setted[old_hl]]), "%*s%s", LR_ARROWS, "", opts[old_hl].list[setted[old_hl]]);
        }
        if(hl == set.len) {
            mvwprintw(menu_win, hl+BOX_PADN+1, BOX_PADW, "%*s", PAD_HL, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", last);
        } else {
            mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", PAD_HL, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", set.list[hl]);
            mvwattrprintw(menu_win, hl+BOX_PADN, win_width-BOX_DIM-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), A_STANDOUT, "◄ %s ►", opts[hl].list[setted[hl]]);
        }

        old_hl = hl;

        key = wgetch(menu_win);
        switch (key) {
            case KEY_UP:
            case KEY_PPAGE:
                if (--hl < 0) {
                    hl = set.len;
                }
                break;
            
            case KEY_DOWN:
            case KEY_NPAGE:
                if (++hl > set.len) {
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
                hl = set.len;
                break;

            case ENTER:
                if(hl == set.len) {
                    unwin(menu_win);
                    return;
                }
                break;

            default:
                // Check first letters
                for(i = 0; i < set.len; i++) {
                    if(key == set.list[i][0] || key == set.list[i][0]+DIFF_CAPITAL) {
                        hl = i;
                        break;
                    }
                }
                if(key == last[0] || key == last[0]+DIFF_CAPITAL) {
                    hl = set.len;
                }
                break;
        }
    }
}

// Home Menu function
void home_menu(void) {
    str chosen;
    str list[] = HOME_SET;
    List_str set;
    set.list = list;
    set.len = N_HOME_SET;

    while(TRUE) {
        chosen = menu(TITLE, set);
        if(strcmp(chosen, NEW_GAME) == 0) {
            game();
        } else if(strcmp(chosen, BEST_SCORES) == 0) {
            best_scores();
        } else if(strcmp(chosen, SETTINGS) == 0) {
            settings();
        } else if(strcmp(chosen, CREDITS) == 0) {
            credits();
        } else {
            return;
        }
    }
}

// Best scores screen
void best_scores(void) {
    
}

// Credits screen
void credits(void) {
    str list0[] = CREDITS_SET;
    str list1[] = {"", "", "", ""};
    List_str sx;
    sx.list = list0;
    sx.len = N_CREDITS_SET;
    List_str dx;
    dx.list = list1;
    dx.len = N_CREDITS_SET;
    view(CREDITS, sx, dx);
}

int pause(void) {
    str list[] = PAUSE_SET;
    List_str set;
    set.list = list;
    set.len = N_PAUSE_SET;
    menu(PAUSE, set);
    return 0;
}

void endgame(int score) {
    
}
