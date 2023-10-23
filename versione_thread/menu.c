// Include libs
#include <curses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "resources.h"

// Define constants
#define BOX_PADN 2 // North padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADS 1 // South padding of the box
#define HL_PADX 1 // Highlight padding
#define SEL_PADY 1 // Empty lines in settings
#define LR_ARROWS 4 // N chars occupied by arrows in settings

void view(str title, List_str sx, List_str dx) {
    if(sx.len != dx.len) { // Check equal length
        return;
    }
    int i, win_width = max(max_strlen(sx, 0)+max_strlen(dx, 0), strlen(title)) + BOX_PADW + BOX_PADE; // Calc window width
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
                // Check first letters from hl
                for(i = (hl+1)%set.len; i != hl; i = (i+1)%set.len) {
                    if(key == set.list[i][0] || key == set.list[i][0]+DIFF_CAPITAL) {
                        hl = i;
                        break;
                    }
                }
                break;
        }
    }
}

// Settings Menu
void settings_menu(void) {
    // Init vars, settings, options
    int i, set_width, opts_width = 0, win_width, key, hl = 0, old_hl = 0;
    str set0[] = {LANGUAGE, DIFFICULTY, SKIN};
    str sel0[] = {APPLY, CANCEL};
    str language[] = {LANGUAGE_0, LANGUAGE_1};
    str difficulty[] = {DIFFICULTY_0, DIFFICULTY_1, DIFFICULTY_2};
    str skin[] = {SKIN_0, SKIN_1, SKIN_2};

    List_str set; // Settings
    set.list = set0;
    set.len = N_SETTINGS;
    List_str sel; // Selectables
    sel.list = sel0;
    sel.len = N_SETTINGS_SEL;

    List_str opts[N_SETTINGS]; // Options for each settings
    opts[0].list = language;
    opts[0].len = N_LANGUAGE;
    opts[1].list = difficulty;
    opts[1].len = N_DIFFICULTY;
    opts[2].list = skin;
    opts[2].len = N_SKIN;

    int newly_setted[N_SETTINGS]; // Settings before apply
    for(i = 0; i < N_SETTINGS; i++) {
        newly_setted[i] = game_settings[i];
    }

    // Calc window width
    set_width = max_strlen(set, max_strlen(sel, strlen(SETTINGS)));
    for(i = 0; i < set.len; i++) {
        opts_width = max_strlen(opts[i], opts_width);
    }
    win_width = set_width + opts_width + LR_ARROWS + BOX_PADW + BOX_PADE;

    WINDOW* menu_win = newctrwin(set.len+sel.len+BOX_PADN+BOX_PADS+SEL_PADY, win_width); // Create centered window
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, SETTINGS); // Print title

    for(i = 0; i < set.len; i++) { // Print sets, opts
        mvwfattrprintw(menu_win, i+BOX_PADN, BOX_PADW, A_UNDERLINE, set.list[i]);
        mvwprintw(menu_win, i+BOX_PADN, win_width-BOX_PADW-strlen(opts[i].list[newly_setted[i]]), "%s", opts[i].list[newly_setted[i]]);
    }
    for(i = 0; i < sel.len; i++) { // Print sels
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
            mvwprintw(menu_win, old_hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[old_hl].list[newly_setted[old_hl]]), "%*s%s", LR_ARROWS, "", opts[old_hl].list[newly_setted[old_hl]]);
        }
        if(hl >= set.len) { // If hl referes to a selectable...
            mvwprintw(menu_win, hl+BOX_PADN+SEL_PADY, BOX_PADW, "%*s", HL_PADX, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", sel.list[hl-set.len]);
        } else {
            mvwprintw(menu_win, hl+BOX_PADN, BOX_PADW, "%*s", HL_PADX, "");
            wattrprintw(menu_win, A_STANDOUT, "%s", set.list[hl]);
            mvwattrprintw(menu_win, hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[newly_setted[hl]]), A_STANDOUT, "◄ %s ►", opts[hl].list[newly_setted[hl]]);
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
                if(hl < set.len) { // If hl is a settings delete old option highlighted
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[newly_setted[hl]]), "%*s", (int) strlen(opts[hl].list[newly_setted[hl]])+LR_ARROWS, "");
                    if(--newly_setted[hl] < 0) {
                        newly_setted[hl] = opts[hl].len - 1;
                    }
                }
                break;

            case KEY_RIGHT:
                if(hl < set.len) { // If hl is a settings delete old option highlighted
                    mvwprintw(menu_win, hl+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[newly_setted[hl]]), "%*s", (int) strlen(opts[hl].list[newly_setted[hl]])+LR_ARROWS, "");
                    if(++newly_setted[hl] >= opts[hl].len) {
                        newly_setted[hl] = 0;
                    }
                }
                break;

            case KEY_HOME:
                hl = 0;
                break;

            case KEY_END:
                hl = set.len + sel.len - 1;
                break;

            case ENTER:
                if(hl >= set.len) { // If hl is a selectable...
                    if(hl == set.len) {
                        for(i = 0; i < N_SETTINGS; i++) {
                            game_settings[i] = newly_setted[i];
                        }
                        wr_settings();
                    }
                    unwin(menu_win);
                    return;
                }
                break;

            default:
                // Check first letters from hl
                for(i = (hl+1)%(set.len+sel.len); i != hl; i = (i+1)%(set.len+sel.len)) {
                    if(i < set.len) {
                        if(key == set.list[i][0] || key == set.list[i][0]+DIFF_CAPITAL) {
                            hl = i;
                            break;
                        }
                    } else {
                        if(key == sel.list[i-set.len][0] || key == sel.list[i-set.len][0]+DIFF_CAPITAL) {
                            hl = i;
                            break;
                        }
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
    set.len = N_HOME;

    int chosen = menu(TITLE, set);
    switch(chosen) {
        case 0:
            game();
            break;

        case 1:
            best_scores_menu();
            break;

        case 2:
            settings_menu();
            break;

        case 3:
            credits_menu();
            break;
            
        default:
            endwin();
            exit(0);            
    }
}

// Best scores screen
void best_scores_menu(void) {
    int i;
    UserScore* best = rd_best(); // Retreive best scores
    str users[N_BEST], scores[N_BEST];
    List_str sx, dx;
    sx.list = users;
    dx.list = scores;
    sx.len = dx.len = N_BEST;
    for(i = 0; i < N_BEST; i++) {
        if(best[i].score >= 0) {
            sx.list[i] = best[i].user;
            dx.list[i] = int_to_str(best[i].score);
        } else {
            sx.list[i] = dx.list[i] = "";
        }
    }
    view(BEST_SCORES, sx, dx);

    // Free memory
    for(i = 0; i < N_BEST; i++) {
        free(best[i].user);
    }
    free(best);
}

// Credits screen
void credits_menu(void) {
    str list0[] = {PROJECT, "", FRANCESCO, MATTEO};
    str list1[] = {"", "", "", ""};
    List_str sx, dx;
    sx.list = list0;
    dx.list = list1;
    sx.len = dx.len = N_CREDITS;
    view(CREDITS, sx, dx);
}

int pause_menu(void) {
    str list[] = {RESUME, NEW_GAME, SETTINGS, HOME, QUIT};
    List_str set;
    set.list = list;
    set.len = N_PAUSE;
    menu(PAUSE, set);
    return 0;
}

void endgame_menu(int score) {
    
}
