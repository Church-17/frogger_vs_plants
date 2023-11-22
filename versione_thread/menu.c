// Include libs
#include "game.h"
#include "menu.h"
#include "res.h"
#include "utils.h"

// Define constants
#define LR_ARROWS 4 // N chars occupied by arrows in settings
#define TITLE_ROW 1 // Row where start to print title
#define BOX_PADN 1 // North padding of the box
#define BOX_PADX 2 // West & east padding of the box
#define BOX_PADX_ADD 3 // Adding space on x-axis of the box
#define BOX_PADS 1 // South padding of the box
#define HL_PADX 1 // Highlight padding
#define SET_PADY 0 // Empty lines between settings
#define SET_SEL_PADY 1 // Empty lines between settings and selectables

// Define macros
#define WIN_WIDTH(sx, dx, title) (max((sx)+(dx), title)+(2*BOX_PADX)+(BOX_PADX_ADD)) // Calc win_width partially
#define POSITION_Y(ind, lim, title) ((ind)+(SET_PADY)*((ind)+1)+(TITLE_ROW)+(title)+(BOX_PADN)+(SET_SEL_PADY)*(ind>=lim)) // Calc lines of each set in mv function (check to add lines)
#define POSITION_X_DX(obj, win_width) ((win_width)-(BOX_PADX)-strlen(obj)) // Calc dx cols of each option in mv function

// Function prototypes
WINDOW* init_menu(List_str title, int rows, int cols);
void view(List_str title, List_str sx, List_str dx, List_attr cols);
int menu(List_str title, List_str opts);
void mvctrwin(WINDOW* win);
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr);
void check_key(int key, int* hl, List_str* set);

// General function for styled double column view
void view(List_str title, List_str sx, List_str dx, List_attr attrs) {
    // Init vars & setup window
    int i;
    int win_width = WIN_WIDTH(max_strlen(sx, 0), max_strlen(dx, 0), max_strlen(title, 0));
    int win_height = POSITION_Y(sx.len, sx.len+1, title.len)+BOX_PADS;
    WINDOW* menu_win = init_menu(title, win_height, win_width); // Init centered menu
    
    // Print lists with attributes
    for(i = 0; i < sx.len; i++) {
        mvwattrprintw(menu_win, POSITION_Y(i, sx.len, title.len), BOX_PADX, attrs.list[i], "%s", sx.list[i]); // Print elements of sx column
        mvwattrprintw(menu_win, POSITION_Y(i, sx.len, title.len), POSITION_X_DX(dx.list[i], win_width), attrs.list[i], "%s", dx.list[i]); // Print elements of dx column
    }

    wgetch(menu_win); // Press any key to exit
    unwin(menu_win);
}

// General function for a single column menu, returning index of selected option
int menu(List_str title, List_str set) {
    // Init vars & setup window
    int i, key, inc, hl = 0, old_hl = 0;
    int win_width = WIN_WIDTH(max_strlen(set, 0), 0, max_strlen(title, 0)) + HL_PADX; // Calc window width
    int win_height = POSITION_Y(set.len, set.len+1, title.len)+BOX_PADS; // Calc window height
    WINDOW* menu_win = init_menu(title, win_height, win_width); // Init centered menu
    
    // Print options with first letter underlined
    for(i = 0; i < set.len; i++) {
        mvwfattrprintw(menu_win, POSITION_Y(i, set.len, title.len), BOX_PADX, A_UNDERLINE, set.list[i]);
    }

    // Loop to get pressed key
    while(TRUE) {
        // Update highlighted & non-highlighted option
        mvwfattrprintw(menu_win, POSITION_Y(old_hl, set.len, title.len), BOX_PADX, A_UNDERLINE, set.list[old_hl]);
        wprintw(menu_win, "%*s", HL_PADX, ""); // Delete old_hl padding
        mvwprintw(menu_win, POSITION_Y(hl, set.len, title.len), BOX_PADX, "%*s", HL_PADX, ""); // Print hl padding
        wattroff(menu_win, COLS1);
        wattrprintw(menu_win, A_STANDOUT | COLS2, "%s", set.list[hl]);
        wattron(menu_win, COLS1);

        old_hl = hl; // Track old hl
        inc = 1; // Restore inc

        key = wgetch(menu_win); // Get pressed key
        switch (key) {
            // Change hl
            case KEY_UP:
            case KEY_LEFT:
            case KEY_PPAGE:
                inc = -1;
            case KEY_DOWN:
            case KEY_RIGHT:
            case KEY_NPAGE:
                hl = mod(hl+inc, set.len);
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
            
            case KEY_RESIZE:
                mvctrwin(menu_win);
                break;

            default:
                // Check numbers & first letter
                check_key(key, &hl, &set);
                break;
        }
    }
}

// Home Menu
void home_menu(void) {
    // Init vars
    str tit[] = {TITLE};
    List_str title;
    title.list = tit;
    title.len = 1;
    str list[N_HOME] = {NEW_GAME, BEST_SCORES, SETTINGS, CREDITS, QUIT};
    int ind[N_HOME] = {HOME_GAME_ID, HOME_BEST_ID, HOME_SETT_ID, HOME_CRED_ID, HOME_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_HOME);

    int chosen = menu(title, set); // Call menu
    switch(chosen) {
        case HOME_GAME_ID: // Game
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

        case HOME_QUIT_ID:
            quit(NO_ERR);

        default:
            break;
    }

    free(set.list); // Free memory

    return;
}

// Best scores screen
void best_scores_menu(void) {
    // Init vars
    int i;
    str tit[] = {BEST_SCORES};
    List_str title;
    title.list = tit;
    title.len = 1;
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
    for(i = 0; i < best.len; i++) {
        sx.list[i] = best.key[i];
        alloc(char, dx.list[i], LEN_STR_INT);
        sprintf(dx.list[i], "%d", best.val[i]);
    }
    for(i = best.len; i < N_BEST; i++) {
        sx.list[i] = dx.list[i] = ""; // Print empty line
    }
    view(title, sx, dx, cols); // Call view

    // Free memory
    for(i = 0; i < best.len; i++) {
        free(dx.list[i]);
        free(best.key[i]);
    }
    if(best.len > 0) {
        free(best.key);
        free(best.val);
    }
}

// Settings Menu
void settings_menu(void) {
    // Init vars
    int i, key, inc, hl = 0, old_hl = 0;
    // Title
    str tit[] = {SETTINGS};
    List_str title;
    title.list = tit;
    title.len = 1;
    // Settings
    str set0[N_SET_SEL] = {LANGUAGE, DIFFICULTY, SKIN, FIRST_COLOR, SECOND_COLOR, APPLY, CANCEL};
    int ind_set[N_SET_SEL] = {SET_LANG_ID, SET_DIFF_ID, SET_SKIN_ID, SET_COL1_ID, SET_COL2_ID, SET_APPL_ID, SET_CANC_ID};
    List_str set = dict_to_list(set0, ind_set, N_SET_SEL);
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

    // Calc window width & height
    int opts_width = 0;
    for(i = 0; i < N_SETTINGS; i++) {
        opts_width = max_strlen(opts[i], opts_width);
    }
    int win_width = WIN_WIDTH(max_strlen(set, 0), opts_width, max_strlen(title, 0)) + HL_PADX + LR_ARROWS;
    int win_height = POSITION_Y(set.len, N_SETTINGS, title.len)+BOX_PADS;

    // Setup window
    WINDOW* menu_win = init_menu(title, win_height, win_width); // Init centered menu

    // Prints
    for(i = 0; i < set.len; i++) { // Settings & selectables
        mvwfattrprintw(menu_win, POSITION_Y(i, N_SETTINGS, title.len), BOX_PADX, A_UNDERLINE, set.list[i]);
    }
    for(i = 0; i < N_SETTINGS; i++) { // Options
        mvwprintw(menu_win, POSITION_Y(i, N_SETTINGS, title.len), POSITION_X_DX(opts[i].list[newly_setted[i]], win_width), "%s", opts[i].list[newly_setted[i]]);
    }
    
    // Loop to get the pressed key
    while(TRUE) {
        // Update old_hl to become non-highlighted
        mvwfattrprintw(menu_win, POSITION_Y(old_hl, N_SETTINGS, title.len), BOX_PADX, A_UNDERLINE, set.list[old_hl]);
        wprintw(menu_win, "%*s", HL_PADX, ""); // Fix for HL_PADX
        if(old_hl < N_SETTINGS) { // If old_hl referes to a setting...
            mvwprintw(menu_win, POSITION_Y(old_hl, N_SETTINGS, title.len), POSITION_X_DX(opts[old_hl].list[newly_setted[old_hl]], win_width)-LR_ARROWS, "%*s%s", LR_ARROWS, "", opts[old_hl].list[newly_setted[old_hl]]);
        }
        // Update hl to become highlighted
        wattroff(menu_win, COLS1);
        mvwprintw(menu_win, POSITION_Y(hl, N_SETTINGS, title.len), BOX_PADX, "%*s", HL_PADX, ""); // Print HL_PADX
        wattrprintw(menu_win, A_STANDOUT | COLS2, "%s", set.list[hl]);
        if(hl < N_SETTINGS) { // If hl referes to a setting...
            mvwattrprintw(menu_win, POSITION_Y(hl, N_SETTINGS, title.len), POSITION_X_DX(opts[hl].list[newly_setted[hl]], win_width)-LR_ARROWS, A_STANDOUT | COLS2, "◄ %s ►", opts[hl].list[newly_setted[hl]]);
        }
        wattron(menu_win, COLS1);

        old_hl = hl; // Track old hl
        inc = 1; // Restore inc

        key = wgetch(menu_win);
        switch (key) {
            // Change hl
            case KEY_UP:
            case KEY_PPAGE:
                inc = -1; // Decrease
            case KEY_DOWN:
            case KEY_NPAGE:
                hl = mod(hl+inc, set.len);
                break;

            // Change selected option
            case KEY_LEFT:
                inc = -1; // Decrease
            case KEY_RIGHT:
                if(hl < N_SETTINGS) { // If hl is a settings...
                    mvwprintw(menu_win, POSITION_Y(hl, N_SETTINGS, title.len), POSITION_X_DX(opts[hl].list[newly_setted[hl]], win_width)-LR_ARROWS, "%*s", (int)strlen(opts[hl].list[newly_setted[hl]])+LR_ARROWS, ""); // Delete old corrispondent option
                    newly_setted[hl] = mod(newly_setted[hl]+inc, opts[hl].len);
                }
                break;

            // Highlight first setting
            case KEY_HOME:
                hl = 0;
                break;

            // Highlight last selectable
            case KEY_END:
                hl = set.len-1;
                break;

            case ENTER:
                if(hl < N_SETTINGS || hl == SET_APPL_ID) { // If is apply...
                    wr_settings(newly_setted); // Update game settings
                }
                unwin(menu_win);
                return;

            case KEY_RESIZE:
                mvctrwin(menu_win);
                break;

            default:
                // Check numbers & first letter
                check_key(key, &hl, &set);
                break;
        }
    }
    
    // Free memory
    free(set.list);
    for(i = 0; i < N_SETTINGS; i++) {
        free(opts[i].list);
    }
}

// Credits screen
void credits_menu(void) {
    // Init vars
    str tit[] = {CREDITS, PROJECT};
    List_str title;
    title.list = tit;
    title.len = 2;
    str list0[N_CREDITS] = {FRANCESCO, MATTEO};
    str list1[N_CREDITS] = {"", ""};
    attr_t list2[N_CREDITS] = {COLS1, COLS1};
    List_str sx, dx;
    List_attr cols;
    sx.list = list0;
    dx.list = list1;
    cols.list = list2;
    cols.len = sx.len = dx.len = N_CREDITS;

    view(title, sx, dx, cols);
}

int pause_menu(void) {
    // Init vars
    str tit[] = {PAUSE};
    List_str title;
    title.list = tit;
    title.len = 1;
    str list[N_PAUSE] = {RESUME, RETRY, BACK_HOME, QUIT};
    int ind[N_PAUSE] = {PAUSE_RES_ID, PAUSE_RETR_ID, PAUSE_BACK_ID, PAUSE_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_PAUSE);
    
    int chosen = menu(title, set);
    free(set.list);
    return chosen;
}

int gameover_menu(int score) {
    char scorestr[LIM_STR_BUFF];
    sprintf(scorestr, "%s: %d", SCORE, score);
    str tit[] = {GAMEOVER, scorestr};
    List_str title;
    title.list = tit;
    title.len = 2;
    str list[N_OVER] = {RETRY, BACK_HOME, QUIT};
    int ind[N_OVER] = {OVER_RETR_ID, OVER_BACK_ID, OVER_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_OVER);

    int chosen = menu(title, set); // Call menu
    free(set.list); // Free memory
    return chosen;
}

// Init menu window
WINDOW* init_menu(List_str title, int rows, int cols) {
    check_term();
    WINDOW* menu_win = newwin(rows, cols, (LINES - rows)/2, (COLS - cols)/2); // Centered window
    keypad(menu_win, TRUE); // Enable function keys listener
    wattron(menu_win, COLS1); // Enable chosen color
    box(menu_win, ACS_VLINE, ACS_HLINE); // Box window
    for(int i = 0; i < title.len; i++) {
        wctrprintw(menu_win, TITLE_ROW+i, " %s ", title.list[i]); // Print title
    }
    return menu_win;
}

// Check if term is large enough
void check_term() {
    if(LINES < MIN_ROWS || COLS < MIN_COLS) {
        WINDOW* err_win = newwin(LINES, COLS, 0, 0);
        keypad(err_win, TRUE);
        mvwprintw(err_win, 0, 0, "Expand terminal:");
        mvwprintw(err_win, 1, 0, "Minimum: %d x %d ", MIN_ROWS, MIN_COLS);
        while(LINES < MIN_ROWS || COLS < MIN_COLS) {
            mvwprintw(err_win, 2, 0, "Actual:  %d x %d ", LINES, COLS);
            wgetch(err_win);
        }
        unwin(err_win);
    }
}

// Move window in central
void mvctrwin(WINDOW* win) {
    mvwin(win, (LINES - win->_maxy)/2, (COLS - win->_maxx)/2); // Move window to center position
    clear(); // Remove garbage
    refresh();
}

// Move & print string with first letter attributed
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr) {
    mvwattrprintw(win, row, col, attr, "%c", fstr[0]);
    wprintw(win, "%s", &(fstr[1]));
}

// Check if key is one of the first letters
void check_key(int key, int* hl, List_str* set) {
    if(key >= KEY_0 && key <= KEY_9 && key-KEY_0 < set->len) {
        *hl = key-KEY_0;
        return;
    }
    for(int i = mod(*hl+1, set->len); i != *hl; i = mod(i+1, set->len)) {
        if(key == set->list[i][0] || key == set->list[i][0]+CAPITAL_SHIFT) {
            *hl = i;
            return;
        }
    }
}
