// Include libs
#include <ncurses.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "res.h"

// Define constants
#define LR_ARROWS 4 // # chars occupied by arrows in settings
#define BOX_PADN 2 // North padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADS 1 // South padding of the box
#define HL_PADX 1 // Highlight padding
#define SET_PADY 0 // Empty lines between settings
#define SET_SEL_PADY 1 // Empty lines between settings and selectables
#define POSITION_Y(obj) (obj)+SET_PADY*((obj)+1)+BOX_PADN // Calc lines in mv function
#define POSITION_X_DX(obj, win_width) win_width-BOX_PADW-strlen(obj) // Calc dx cols in mv function (needs win_width)

// General function for styled double column view
void view(str title, List_str sx, List_str dx, List_attr attrs) {
    int i, win_width = max(max_strlen(sx, 0)+max_strlen(dx, 0), strlen(title)) + BOX_PADW + BOX_PADE; // Calc window width
    WINDOW* menu_win = newctrwin(POSITION_Y(sx.len)+BOX_PADS, win_width); // Centered window
    wattron(menu_win, COLS1);
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, title); // Print title

    // Print lists with attributes
    for(i = 0; i < sx.len; i++) {
        mvwattrprintw(menu_win, POSITION_Y(i), BOX_PADW, attrs.list[i], "%s", sx.list[i]);
        mvwattrprintw(menu_win, POSITION_Y(i), POSITION_X_DX(dx.list[i], win_width), attrs.list[i], "%s", dx.list[i]);
    }
    wgetch(menu_win); // Press any key to exit
    unwin(menu_win);
}

// General function for a single column menu, returning index of selected option
int menu(str title, List_str set) {
    // Init vars
    int i, key, hl = 0, old_hl = 0;
    WINDOW* menu_win = newctrwin(POSITION_Y(set.len)+BOX_PADS, max_strlen(set, strlen(title))+BOX_PADW+BOX_PADE); // Create centered window

    // Setup window
    wattron(menu_win, COLS1);
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, title); // Print title

    // Print options with first letter underlined
    for(i = 0; i < set.len; i++) {
        mvwfattrprintw(menu_win, POSITION_Y(i), BOX_PADW, A_UNDERLINE, set.list[i]);
    }

    // Loop to get pressed key
    while(TRUE) {
        // Update highlighted & non-highlighted option
        mvwfattrprintw(menu_win, POSITION_Y(old_hl), BOX_PADW, A_UNDERLINE, set.list[old_hl]);
        wprintw(menu_win, "%*s", HL_PADX, ""); // Delete old_hl padding
        mvwprintw(menu_win, POSITION_Y(hl), BOX_PADW, "%*s", HL_PADX, ""); // Print hl padding
        wattroff(menu_win, COLS1);
        wattrprintw(menu_win, A_STANDOUT | COLS2, "%s", set.list[hl]);
        wattron(menu_win, COLS1);

        old_hl = hl; // Track old hl

        key = wgetch(menu_win); // Get the pressed key
        switch (key) {
            // Decrease hl
            case KEY_UP:
            case KEY_LEFT:
            case KEY_PPAGE:
                hl = mod(hl-1, set.len);
                break;
            
            // Increase hl
            case KEY_DOWN:
            case KEY_RIGHT:
            case KEY_NPAGE:
                hl = mod(hl+1, set.len);
                break;

            // Highlight first option
            case KEY_HOME:
                hl = 0;
                break;

            // Highlight last option 
            case KEY_END:
                hl = set.len-1;
                break;

            // Select the highlighted option
            case ENTER:
                unwin(menu_win);
                return hl;

            default:
                // Check first letters of options
                for(i = mod(hl+1, set.len); i != hl; i = mod(i+1, set.len)) {
                    if(key == set.list[i][0] || key == set.list[i][0]+CAPITAL_SHIFT) {
                        hl = i;
                        break;
                    }
                }
                break;
        }
    }
}

// Home Menu
void home_menu(void) {
    // Init vars
    str list[N_HOME] = {NEW_GAME, BEST_SCORES, SETTINGS, CREDITS, QUIT};
    int ind[N_HOME] = {HOME_NEWG_ID, HOME_BEST_ID, HOME_SETT_ID, HOME_CRED_ID, HOME_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_HOME);

    int chosen = menu(TITLE, set); // Call menu
    switch(chosen) {
        case HOME_NEWG_ID: // Game
            game();
            break;

        case HOME_BEST_ID: // Best scores
            best_scores_menu();
            break;

        case HOME_SETT_ID: // Settings
            settings_menu();
            break;

        case HOME_CRED_ID: // Credits
            credits_menu();
            break;
            
        case HOME_QUIT_ID: // Quit
            endwin();
            exit(0); 

        default:
            break;           
    }

    // Free memory
    free(set.list);
}

// Best scores screen
void best_scores_menu(void) {
    // Init vars
    int i;
    str users[N_BEST], scores[N_BEST];
    attr_t list0[N_BEST] = {YELLOW_BLACK, GREY_BLACK, BROWN_BLACK};
    List_str sx, dx;
    List_attr cols;
    sx.list = users;
    dx.list = scores;
    cols.list = list0;
    cols.len = sx.len = dx.len = N_BEST;

    // Pass best scores ignoring empty scores
    Dict_str_int best = rd_best(); // Retreive best scores
    for(i = 0; i < N_BEST; i++) {
        if(best.val[i] > NULL_RECORD) {
            sx.list[i] = best.key[i];
            dx.list[i] = int_to_str(best.val[i]);
        } else { // If a score is negative, it's a non-existing record
            sx.list[i] = dx.list[i] = ""; // Print empty line
        }
    }
    view(BEST_SCORES, sx, dx, cols); // Call view

    // Free memory
    for(i = 0; i < N_BEST; i++) {
        if(best.val[i] > NULL_RECORD) {
            free(dx.list[i]);
        }
        free(best.key[i]);
    }
    free(best.key);
    free(best.val);
}

// Settings Menu
void settings_menu(void) {
    // Init vars
    int i, key, hl = 0, old_hl = 0;
    // Settings
    str set0[N_SETTINGS] = {LANGUAGE, DIFFICULTY, SKIN, FIRST_COLOR, SECOND_COLOR};
    int ind_set[N_SETTINGS] = {SET_LANG_ID, SET_DIFF_ID, SET_SKIN_ID, SET_COL1_ID, SET_COL2_ID};
    List_str set = dict_to_list(set0, ind_set, N_SETTINGS);
    // Selectables
    str sel0[N_SETTINGS_SEL] = {APPLY, CANCEL};
    int ind_sel[N_SETTINGS_SEL] = {SET_APPL_ID, SET_CANC_ID};
    List_str sel = dict_to_list(sel0, ind_sel, N_SETTINGS_SEL);
    // Options for each settings
    List_str opts[N_SETTINGS];
    str language[N_LANGUAGE] = {LANGUAGE_0, LANGUAGE_1};
    int ind_lang[N_LANGUAGE] = {LANG_0_ID, LANG_1_ID};
    opts[SET_LANG_ID] = dict_to_list(language, ind_lang, N_LANGUAGE);
    str difficulty[N_DIFFICULTY] = {DIFFICULTY_0, DIFFICULTY_1, DIFFICULTY_2};
    int ind_diff[N_DIFFICULTY] = {DIFF_0_ID, DIFF_1_ID, DIFF_2_ID};
    opts[SET_DIFF_ID] = dict_to_list(difficulty, ind_diff, N_DIFFICULTY);
    str skin[N_SKIN] = {SKIN_0, SKIN_1, SKIN_2};
    int ind_skin[N_SKIN] = {SKIN_0_ID, SKIN_1_ID, SKIN_2_ID};
    opts[SET_SKIN_ID] = dict_to_list(skin, ind_skin, N_SKIN);
    str color[N_COLOR] = {COLOR_0, COLOR_1, COLOR_2, COLOR_3, COLOR_4, COLOR_5, COLOR_6};
    int ind_col[N_COLOR] = {COL_0_ID, COL_1_ID, COL_2_ID, COL_3_ID, COL_4_ID, COL_5_ID, COL_6_ID};
    opts[SET_COL2_ID] = opts[SET_COL1_ID] = dict_to_list(color, ind_col, N_COLOR);

    // Sync newly setted to settings
    int newly_setted[N_SETTINGS];
    for(i = 0; i < N_SETTINGS; i++) {
        newly_setted[i] = game_settings[i];
    }

    // Calc window width
    int set_width = max_strlen(set, max_strlen(sel, 0)), opts_width = 0;
    for(i = 0; i < set.len; i++) {
        opts_width = max_strlen(opts[i], opts_width);
    }
    int win_width = max(set_width+opts_width, strlen(SETTINGS)) + LR_ARROWS + BOX_PADW + BOX_PADE;

    // Setup window 
    WINDOW* menu_win = newctrwin(POSITION_Y(set.len+sel.len)+BOX_PADS+SET_SEL_PADY, win_width); // Create centered window
    wattron(menu_win, COLS1);
    keypad(menu_win, TRUE); // Enable function keys listener
    box(menu_win, 0, 0); // Box window
    wctrprintw(menu_win, 0, SETTINGS); // Print title

    // Prints
    for(i = 0; i < set.len; i++) { // Settings & options
        mvwfattrprintw(menu_win, POSITION_Y(i), BOX_PADW, A_UNDERLINE, set.list[i]);
        mvwprintw(menu_win, POSITION_Y(i), POSITION_X_DX(opts[i].list[newly_setted[i]], win_width), "%s", opts[i].list[newly_setted[i]]);
    }
    for(i = 0; i < sel.len; i++) { // Selectables
        mvwfattrprintw(menu_win, POSITION_Y(i+set.len)+SET_SEL_PADY, BOX_PADW, A_UNDERLINE, sel.list[i]);
    }
    
    // Loop to get the pressed key
    while(TRUE) {
        // Update old_hl to become non-highlighted
        if(old_hl >= set.len) { // If old_hl referes to a selectable...
            mvwfattrprintw(menu_win, POSITION_Y(old_hl)+SET_SEL_PADY, BOX_PADW, A_UNDERLINE, sel.list[old_hl-set.len]); 
            wprintw(menu_win, "%*s", HL_PADX, ""); // Fix for HL_PADX
        } else { // If old_hl referes to a setting...
            mvwfattrprintw(menu_win, POSITION_Y(old_hl), BOX_PADW, A_UNDERLINE, set.list[old_hl]);
            wprintw(menu_win, "%*s", HL_PADX, ""); // Fix for HL_PADX
            mvwprintw(menu_win, POSITION_Y(old_hl), POSITION_X_DX(opts[old_hl].list[newly_setted[old_hl]], win_width)-LR_ARROWS, "%*s%s", LR_ARROWS, "", opts[old_hl].list[newly_setted[old_hl]]);
        }
        // Update hl to become highlighted or to update corrispondent option
        wattroff(menu_win, COLS1);
        if(hl >= set.len) { // If hl referes to a selectable...
            mvwprintw(menu_win, POSITION_Y(hl)+SET_SEL_PADY, BOX_PADW, "%*s", HL_PADX, ""); // Print HL_PADX
            wattrprintw(menu_win, A_STANDOUT | COLS2, "%s", sel.list[hl-set.len]);
        } else { // If hl referes to a setting...
            mvwprintw(menu_win, POSITION_Y(hl), BOX_PADW, "%*s", HL_PADX, ""); // Print HL_PADX
            wattrprintw(menu_win, A_STANDOUT | COLS2, "%s", set.list[hl]);
            mvwattrprintw(menu_win, POSITION_Y(hl), POSITION_X_DX(opts[hl].list[newly_setted[hl]], win_width)-LR_ARROWS, A_STANDOUT | COLS2, "◄ %s ►", opts[hl].list[newly_setted[hl]]);
        }
        wattron(menu_win, COLS1);

        old_hl = hl; // Track old hl

        key = wgetch(menu_win);
        switch (key) {
            // Decrease hl
            case KEY_UP:
            case KEY_PPAGE:
                hl = mod(hl-1, set.len+sel.len);
                break;
            
            // Increase hl
            case KEY_DOWN:
            case KEY_NPAGE:
                hl = mod(hl+1, set.len+sel.len);
                break;

            // Change selected option
            case KEY_LEFT:
                if(hl < set.len) { // If hl is a settings...
                    // Delete old corrispondent option
                    mvwprintw(menu_win, hl+SET_PADY*(hl+1)+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[newly_setted[hl]]), "%*s", (int) strlen(opts[hl].list[newly_setted[hl]])+LR_ARROWS, "");
                    // Deacrease index option
                    newly_setted[hl] = mod(newly_setted[hl]-1, opts[hl].len);
                }
                break;

            case KEY_RIGHT:
                if(hl < set.len) { // If hl is a settings...
                    // Delete old corrispondent option
                    mvwprintw(menu_win, hl+SET_PADY*(hl+1)+BOX_PADN, win_width-BOX_PADW-LR_ARROWS-strlen(opts[hl].list[newly_setted[hl]]), "%*s", (int) strlen(opts[hl].list[newly_setted[hl]])+LR_ARROWS, "");
                    // Increase index option
                    newly_setted[hl] = mod(newly_setted[hl]+1, opts[hl].len);
                }
                break;

            // Highlight first setting
            case KEY_HOME:
                hl = 0;
                break;

            // Highlight last selectable
            case KEY_END:
                hl = set.len + sel.len - 1;
                break;

            case ENTER:
                if(hl < set.len || hl == set.len+SET_APPL_ID) { // If is apply...
                    // Update game settings
                    wr_settings(newly_setted);
                }
                unwin(menu_win);
                return;

            default:
                // Check first letters from hl
                for(i = mod(hl+1, set.len+sel.len); i != hl; i = mod(i+1, set.len+sel.len)) {
                    if(i < set.len) {
                        if(key == set.list[i][0] || key == set.list[i][0]+CAPITAL_SHIFT) {
                            hl = i;
                            break;
                        }
                    } else {
                        if(key == sel.list[i-set.len][0] || key == sel.list[i-set.len][0]+CAPITAL_SHIFT) {
                            hl = i;
                            break;
                        }
                    }
                }
                break;
        }
    }
    
    // Free memory
    free(set.list);
    free(sel.list);
    for(i = 0; i < N_SETTINGS; i++) {
        free(opts[i].list);
    }
}

// Credits screen
void credits_menu(void) {
    // Init vars
    str list0[N_CREDITS] = {PROJECT, "", FRANCESCO, MATTEO};
    str list1[N_CREDITS] = {"", "", "", ""};
    attr_t list2[N_CREDITS] = {COLS1, COLS1, COLS1, COLS1};
    List_str sx, dx;
    List_attr cols;
    sx.list = list0;
    dx.list = list1;
    cols.list = list2;
    cols.len = sx.len = dx.len = N_CREDITS;

    view(CREDITS, sx, dx, cols);
}

int pause_menu(void) {
    // Init vars
    str list[N_PAUSE] = {RESUME, NEW_GAME, SETTINGS, BACK_HOME, QUIT};
    List_str set;
    set.list = list;
    set.len = N_PAUSE;
    
    menu(PAUSE, set);
    return 0;
}

void endgame_menu(int score) {
    
}
