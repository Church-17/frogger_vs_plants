// Include libs
#include <curses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "strings.h"

void view(str title, List_str sx, List_str dx) {
    if(sx.len != dx.len) { // Check equal length
        return;
    }
    int i, win_width = max_strlen(sx, strlen(title)) + max_strlen(dx, 0) + BOX_PADW + BOX_PADE; // Calc window width
    WINDOW* menu_win = newctrwin(sx.len+BOX_PADN+BOX_PADS, win_width); // Centered window
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, title); // Print title

    // Prints options
    for(i = 0; i < sx.len; i++) {
        mvwprintw(menu_win, i+BOX_PADN, BOX_PADW, "%s", sx.list[i]);
        mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_PADW-strlen(dx.list[i]), "%s", dx.list[i]);
    }
    wgetch(menu_win);
    unwin(menu_win);
}

int menu(str title, List_str set) {
    int i, key, hl = 0, old_hl = 0, win_width = max_strlen(set, strlen(title)) + BOX_PADW + BOX_PADE; // Calc window width
    WINDOW* menu_win = newctrwin(set.len+BOX_PADN+BOX_PADS, win_width); // Centered window
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, title); // Print title

    // Print options with first letter underlined
    for(i = 0; i < set.len; i++) {
        mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[i]);
    }
    while(TRUE) {
        // Update highlighted & non-highlighted option
        mvwfattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[old_hl]);
        wprintw(menu_win, "%*s", HL_PADX, ""); // Delete old_hl padding
        mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", HL_PADX, ""); // Print hl padding
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
                return hl;

            default:
                // Check first letters
                for(i = 0; i < set.len; i++) {
                    if(key == set.list[i][0] || key == set.list[i][0]+DIFF_CAPITAL) {
                        hl = i;
                    }
                }
                break;
        }
    }
}

// Settings Menu
void settings(void) {
    // Init vars, settings, options
    int i, set_width, opts_width = 0, win_width, key, hl = 0, old_hl = 0;
    str set0[] = {LANGUAGE, DIFFICULTY, SKIN};
    str sel0[] = {APPLY, CANCEL};
    str language[] = {ENGLISH, ITALIAN};
    str difficulty[] = {DIFFICULTY_0, DIFFICULTY_1, DIFFICULTY_2};
    str skin[] = {SKIN_0, SKIN_1, SKIN_2};

    List_str set;
    set.list = set0;
    set.len = N_SETTINGS_SET;
    List_str sel;
    sel.list = sel0;
    sel.len = N_SETTINGS_SEL;

    List_str opts[N_SETTINGS_SET];
    opts[0].list = language;
    opts[0].len = N_LANG;
    opts[1].list = difficulty;
    opts[1].len = N_DIFFICULTY_SET;
    opts[2].list = skin;
    opts[2].len = N_SKIN_SET;

    int setted[N_SETTINGS_SET];
    for(i = 0; i < N_SETTINGS_SET; i++) {
        setted[i] = game_params[i];
    }

    // Calc window width
    set_width = max_strlen(set, max_strlen(sel, strlen(SETTINGS)));
    for(i = 0; i < set.len; i++) {
        opts_width = max_strlen(opts[i], opts_width);
    }
    win_width = set_width + opts_width + LR_ARROWS + BOX_PADW + BOX_PADE;

    WINDOW* menu_win = newctrwin(set.len+sel.len+BOX_PADN+BOX_PADS+SEL_PADY, win_width); // Centered window
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, SETTINGS); // Print title

    // Print set, opts, selectables
    for(i = 0; i < set.len; i++) {
        mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[i]);
        mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_PADW-strlen(opts[i].list[setted[i]]), "%s", opts[i].list[setted[i]]);
    }
    for(i = 0; i < sel.len; i++) {
        mvwfattrprintw(menu_win, i+set.len+BOX_PADN+SEL_PADY, BOX_PADW, A_UNDERLINE, sel.list[i]);
    }
    
    while(TRUE) {
        // Update highlighted & non-highlighted option
        if(old_hl >= set.len) { // If old_hl referes to a selectable...
            mvwfattrprintw(menu_win, old_hl+BOX_PADN+SEL_PADY, BOX_PADW, A_UNDERLINE, sel.list[old_hl-set.len]);
            wprintw(menu_win, "%*s", HL_PADX, "");
        } else {
            mvwfattrprintw(menu_win, old_hl+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[old_hl]);
            wprintw(menu_win, "%*s", HL_PADX, "");
            mvwprintw(menu_win, old_hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[old_hl].list[setted[old_hl]]), "%*s%s", LR_ARROWS, "", opts[old_hl].list[setted[old_hl]]);
        }
        if(hl >= set.len) { // If hl referes to a selectable...
            mvwprintw(menu_win, hl+BOX_PADN+SEL_PADY, BOX_PADW, "%*s", HL_PADX, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", sel.list[hl-set.len]);
        } else {
            mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", HL_PADX, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", set.list[hl]);
            mvwattrprintw(menu_win, hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), A_STANDOUT, "◄ %s ►", opts[hl].list[setted[hl]]);
        }

        old_hl = hl; // Track old hl

        key = wgetch(menu_win);
        switch (key) {
            case KEY_UP:
            case KEY_PPAGE:
                if (--hl < 0) {
                    hl = set.len + sel.len - 1;
                }
                break;
            
            case KEY_DOWN:
            case KEY_NPAGE:
                if (++hl >= set.len + sel.len) {
                    hl = 0;
                }
                break;

            case KEY_LEFT:
                if(hl < set.len) { // If hl is a selectable delete old option highlighted
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), "%*s", (int) strlen(opts[hl].list[setted[hl]])+LR_ARROWS, "");
                }
                if(--setted[hl] < 0) {
                    setted[hl] = opts[hl].len - 1;
                }
                break;

            case KEY_RIGHT:
                if(hl < set.len) { // If hl is a selectable delete old option highlighted
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[setted[hl]]), "%*s", (int) strlen(opts[hl].list[setted[hl]])+LR_ARROWS, "");
                }
                if(++setted[hl] >= opts[hl].len) {
                    setted[hl] = 0;
                }
                break;

            case KEY_HOME:
                hl = 0;
                break;

            case KEY_END:
                hl = set.len + sel.len - 1;
                break;

            case ENTER:
                if(hl == set.len) {
                    for(i = 0; i < N_SETTINGS_SET; i++) {
                        game_params[i] = setted[i];
                    }
                }
                unwin(menu_win);
                return;

            default:
                // Check first letters
                for(i = 0; i < set.len; i++) {
                    if(key == set.list[i][0] || key == set.list[i][0]+DIFF_CAPITAL) {
                        hl = i;
                    }
                }
                for(i = 0; i < sel.len; i++) {
                    if(key == sel.list[i][0] || key == sel.list[i][0]+DIFF_CAPITAL) {
                        hl = i + set.len;
                    }
                }
                break;
        }
    }
}

// Home Menu
void home_menu(void) {
    str list[] = {NEW_GAME, BEST_SCORES, SETTINGS, CREDITS, QUIT};
    List_str set;
    set.list = list;
    set.len = N_HOME_SET;

    int chosen = menu(TITLE, set);
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
            credits();
            break;
            
        default:
            endwin();
            exit(0);            
    }
}

// Best scores screen
void best_scores(void) {
    
}

// Credits screen
void credits(void) {
    str list0[] = {PROJECT, "", FRANCESCO, MATTEO};
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
    str list[] = {RESUME, NEW_GAME, SETTINGS, HOME, QUIT};
    List_str set;
    set.list = list;
    set.len = N_PAUSE_SET;
    menu(PAUSE, set);
    return 0;
}

void endgame(int score) {
    
}
