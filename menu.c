// Include libs
#include "main.h"
#include "menu.h"
#include "game.h"
#include "music.h"
#include "str.h"
#include "utils.h"
#include "struct.h"

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
void view(const List_str title, const List_str sx, const List_str dx, const List_attr cols, const Game_t* gamevar);
int menu(const List_str title, const List_str opts, const Game_t* gamevar);
void check_key(int key, int* hl, const List_str set);

// General function for styled double column view
void view(const List_str title, const List_str sx, const List_str dx, const List_attr attrs, const Game_t* gamevar) {
    // Init vars
    bool do_prints = TRUE;
    int win_width = WIN_WIDTH(max_strlen(sx, 0), max_strlen(dx, 0), max_strlen(title, 0));
    int win_height = POSITION_Y(sx.len, sx.len+1, title.len)+BOX_PADS;

    // Setup window
    WINDOW* menu_win = new_ctrwin(win_height, win_width); // Centered window
    wattron(menu_win, COL1); // Enable chosen color

    while(do_prints) {
        // Prints
        box(menu_win, ACS_VLINE, ACS_HLINE); // Box window
        for(int i = 0; i < title.len; i++) { // Print title
            wctrprintw(menu_win, TITLE_ROW+i, " %s ", title.list[i]);
        }
        for(int i = 0; i < sx.len; i++) {
            wattron(menu_win, attrs.list[i]);
            mvwprintw(menu_win, POSITION_Y(i, sx.len, title.len), BOX_PADX, "%s", sx.list[i]); // Print elements of sx column
            mvwprintw(menu_win, POSITION_Y(i, sx.len, title.len), POSITION_X_DX(dx.list[i], win_width), "%s", dx.list[i]); // Print elements of dx column
            wattroff(menu_win, attrs.list[i]);
        }
        do_prints = FALSE;

        while(!do_prints) {
            if(wgetch(menu_win) != KEY_RESIZE) break;
            do_prints = resize_proc(menu_win, win_height, win_width, gamevar);
        }
    }
    un_win(menu_win);
}

// General function for a single column menu, returning index of selected option
int menu(const List_str title, const List_str set, const Game_t* gamevar) {
    // Init vars
    bool do_prints, do_return = FALSE; // Flags
    int key, inc, hl = 0, old_hl = 0;
    int win_width = WIN_WIDTH(max_strlen(set, 0), 0, max_strlen(title, 0)) + HL_PADX; // Calc window width
    int win_height = POSITION_Y(set.len, set.len+1, title.len)+BOX_PADS; // Calc window height

    // Setup window
    WINDOW* menu_win = new_ctrwin(win_height, win_width); // Centered window
    wattron(menu_win, COL1);

    while(!do_return) {
        // Prints
        box(menu_win, ACS_VLINE, ACS_HLINE); // Box window
        for(int i = 0; i < title.len; i++) { // Print title
            wctrprintw(menu_win, TITLE_ROW+i, " %s ", title.list[i]);
        }
        for(int i = 0; i < set.len; i++) { // Print options *** with first letter underlined
            mvwprintw(menu_win, POSITION_Y(i, set.len, title.len), BOX_PADX, "%s", set.list[i]);
        }
        do_prints = FALSE;
        
        while(!(do_prints || do_return)) {
            // Update non-highlighted & highlighted option
            mvwprintw(menu_win, POSITION_Y(old_hl, set.len, title.len), BOX_PADX, "%s%*s", set.list[old_hl], HL_PADX, "");
            mvwprintw(menu_win, POSITION_Y(hl, set.len, title.len), BOX_PADX, "%*s", HL_PADX, ""); // Print hl padding
            wattroff(menu_win, COL1);
            mvwaprintw(menu_win, POSITION_Y(hl, set.len, title.len), BOX_PADX+HL_PADX, A_STANDOUT | COL2, "%s", set.list[hl]);
            wattron(menu_win, COL1);

            old_hl = hl; // Track old hl
            inc = 1; // Restore inc

            key = wgetch(menu_win); // Get pressed key
            switch(key) {
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
                    do_return = TRUE;
                    break;

                case KEY_RESIZE:
                    do_prints = resize_proc(menu_win, win_height, win_width, gamevar);
                    break;

                default:
                    // Check numbers & first letter
                    check_key(key, &hl, set);
                    break;
            }
            // Play selection sound if hl is changed
            if(hl != old_hl) {
                play_sound(MENU_SELECTION_SOUND);
            }
        }
    }
    un_win(menu_win);
    return hl;
}

// Home Menu
int home_menu(void) {
    // Init vars
    str tit[] = {STR_HOME};
    List_str title = {tit, 1};
    str list[N_HOME] = {STR_GAME, STR_BEST, STR_SETTINGS, STR_CREDITS, STR_QUIT};
    int ind[N_HOME] = {HOME_GAME_ID, HOME_BEST_ID, HOME_SETT_ID, HOME_CRED_ID, HOME_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_HOME);

    int chosen = menu(title, set, NULL); // Call menu
    
    free(set.list); // Free memory

    return chosen;
}

// Best scores screen
void best_scores_menu(void) {
    // Init vars
    str tit[] = {STR_BEST};
    List_str title = {tit, 1};
    str users[N_BEST], scores[N_BEST];
    attr_t colors[N_BEST] = {YELLOW_BLACK, GREY_BLACK, ORANGE_BLACK};
    List_str sx = {users, N_BEST}, dx = {scores, N_BEST};
    List_attr attrs = {colors, N_BEST};

    // Pass best scores ignoring empty scores
    Dict_str_int best = rd_best(); // Retreive best scores
    for(int i = 0; i < best.len; i++) {
        sx.list[i] = best.key[i];
        alloc(char, dx.list[i], LEN_STR_INT);
        sprintf(dx.list[i], "%d", best.val[i]);
    }
    for(int i = best.len; i < N_BEST; i++) {
        sx.list[i] = dx.list[i] = ""; // Print empty line
    }
    view(title, sx, dx, attrs, NULL); // Call view

    // Free memory
    for(int i = 0; i < best.len; i++) {
        free(dx.list[i]);
        free(best.key[i]);
    }
    if(best.len >= 0) {
        free(best.key);
        free(best.val);
    }
}

// Settings Menu
void settings_menu(void) {
    // Init vars
    bool do_prints, do_return = FALSE; // Flags
    int key, inc, hl = 0, old_hl = 0;
    // Title
    str tit[] = {STR_SETTINGS};
    List_str title = {tit, 1};
    // Settings
    str set0[N_SET_SEL] = {STR_LANGUAGE, STR_DIFFICULTY, STR_SKIN, STR_COLOR_1, STR_COLOR_2, STR_VOL_MUSIC, STR_VOL_EFFECTS, STR_APPLY, STR_CANCEL};
    int ind_set[N_SET_SEL] = {SET_LANG_ID, SET_DIFF_ID, SET_SKIN_ID, SET_COL1_ID, SET_COL2_ID, SET_VOL_MUS_ID, SET_VOL_EFCT_ID, SET_APPL_ID, SET_CANC_ID};
    List_str set = dict_to_list(set0, ind_set, N_SET_SEL);
    // Options for each settings
    List_str opts[N_SETTINGS];
    str language[N_LANGUAGE] = {STR_LANG_EN, STR_LANG_IT};
    int ind_lang[N_LANGUAGE] = {LANG_EN_ID, LANG_IT_ID};
    opts[SET_LANG_ID] = dict_to_list(language, ind_lang, N_LANGUAGE);
    str difficulty[N_DIFFICULTY] = {STR_DIFFICULTY_0, STR_DIFFICULTY_1, STR_DIFFICULTY_2};
    int ind_diff[N_DIFFICULTY] = {DIFF_0_ID, DIFF_1_ID, DIFF_2_ID};
    opts[SET_DIFF_ID] = dict_to_list(difficulty, ind_diff, N_DIFFICULTY);
    str skin[N_SKIN] = {STR_SKIN_0, STR_SKIN_1};
    int ind_skin[N_SKIN] = {SKIN_0_ID, SKIN_1_ID};
    opts[SET_SKIN_ID] = dict_to_list(skin, ind_skin, N_SKIN);
    str color[N_COLOR] = {STR_COL_WHT, STR_COL_RED, STR_COL_GRN, STR_COL_YLW, STR_COL_BLU, STR_COL_MGT, STR_COL_CYN, STR_COL_GRY, STR_COL_RNG};
    int ind_col[N_COLOR] = {WHITE_BLACK_ID, RED_BLACK_ID, GREEN_BLACK_ID, YELLOW_BLACK_ID, BLUE_BLACK_ID, MAGENTA_BLACK_ID, CYAN_BLACK_ID, GREY_BLACK_ID, ORANGE_BLACK_ID};
    opts[SET_COL2_ID] = opts[SET_COL1_ID] = dict_to_list(color, ind_col, N_COLOR);
    str volume[N_VOLUME] = {"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"};
    opts[SET_VOL_MUS_ID].list = opts[SET_VOL_EFCT_ID].list = volume, opts[SET_VOL_MUS_ID].len = opts[SET_VOL_EFCT_ID].len = N_VOLUME;

    // Sync newly setted to settings
    int newly_setted[N_SETTINGS];
    for(int i = 0; i < N_SETTINGS; i++) {
        newly_setted[i] = game_settings[i];
    }

    // Calc window width & height
    int opts_width = 0;
    for(int i = 0; i < N_SETTINGS; i++) {
        opts_width = max_strlen(opts[i], opts_width);
    }
    int win_width = WIN_WIDTH(max_strlen(set, 0), opts_width, max_strlen(title, 0)) + HL_PADX + LR_ARROWS;
    int win_height = POSITION_Y(set.len, N_SETTINGS, title.len)+BOX_PADS;

    // Setup window
    WINDOW* menu_win = new_ctrwin(win_height, win_width); // Centered window
    wattron(menu_win, COL1);

    while(!do_return) {
        // Prints
        box(menu_win, ACS_VLINE, ACS_HLINE); // Box window
        for(int i = 0; i < title.len; i++) { // Print title
            wctrprintw(menu_win, TITLE_ROW+i, " %s ", title.list[i]);
        }
        for(int i = 0; i < set.len; i++) { // Settings & selectables
            mvwprintw(menu_win, POSITION_Y(i, N_SETTINGS, title.len), BOX_PADX, "%s", set.list[i]);
        }
        for(int i = 0; i < N_SETTINGS; i++) { // Options
            mvwprintw(menu_win, POSITION_Y(i, N_SETTINGS, title.len), POSITION_X_DX(opts[i].list[newly_setted[i]], win_width), "%s", opts[i].list[newly_setted[i]]);
        }
        do_prints = FALSE;

        // Loop to print all when needed
        while(!(do_prints || do_return)) {
            // Update old_hl to become non-highlighted
            mvwprintw(menu_win, POSITION_Y(old_hl, N_SETTINGS, title.len), BOX_PADX, "%s%*s", set.list[old_hl], HL_PADX, "");
            if(old_hl < N_SETTINGS) { // If old_hl referes to a setting...
                mvwprintw(menu_win, POSITION_Y(old_hl, N_SETTINGS, title.len), POSITION_X_DX(opts[old_hl].list[newly_setted[old_hl]], win_width)-LR_ARROWS, "%*s%s", LR_ARROWS, "", opts[old_hl].list[newly_setted[old_hl]]);
            }
            // Update hl to become highlighted
            mvwprintw(menu_win, POSITION_Y(hl, N_SETTINGS, title.len), BOX_PADX, "%*s", HL_PADX, ""); // Print HL_PADX
            wattroff(menu_win, COL1);
            wattron(menu_win, A_STANDOUT | COL2);
            wprintw(menu_win, "%s", set.list[hl]);
            if(hl < N_SETTINGS) { // If hl referes to a setting...
                mvwprintw(menu_win, POSITION_Y(hl, N_SETTINGS, title.len), POSITION_X_DX(opts[hl].list[newly_setted[hl]], win_width)-LR_ARROWS, "◄ %s ►", opts[hl].list[newly_setted[hl]]);
            }
            wattroff(menu_win, A_STANDOUT | COL2);
            wattron(menu_win, COL1);

            old_hl = hl; // Track old hl
            inc = 1; // Restore inc

            key = wgetch(menu_win);
            switch(key) {
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
                    do_return = TRUE;
                    break;

                case KEY_RESIZE:
                    do_prints = resize_proc(menu_win, win_height, win_width, NULL);
                    break;

                default:
                    // Check numbers & first letter
                    check_key(key, &hl, set);
                    break;
            }
            // Play selection sound if hl is changed
            if(hl != old_hl) {
                play_sound(MENU_SELECTION_SOUND);
            }
        }
    }
    
    // Free memory
    free(set.list);
    free(opts[SET_LANG_ID].list);
    free(opts[SET_DIFF_ID].list);
    free(opts[SET_SKIN_ID].list);
    free(opts[SET_COL1_ID].list);
    
    // Update game settings if applied
    if(hl < N_SETTINGS || hl == SET_APPL_ID) {
        wr_settings(newly_setted);
        sound_volume(VOL_MUS_SET, VOL_EFCT_SET);
    }
    un_win(menu_win);
}

// Credits Screen
void credits_menu(void) {
    // Init vars
    str tit[] = {STR_CREDITS, STR_PROJECT};
    List_str title = {tit, 2};
    str names[N_CREDITS] = {STR_FRANCESCO, STR_MATTEO};
    str empty[N_CREDITS] = {"", ""};
    attr_t colors[N_CREDITS] = {COL1, COL1};
    List_str sx = {names, N_CREDITS}, dx = {empty, N_CREDITS};
    List_attr attrs = {colors, N_CREDITS};

    view(title, sx, dx, attrs, NULL);
}

// Pause Menu
int pause_menu(const Game_t* gamevar) {
    // Init vars
    str tit[] = {STR_PAUSE};
    List_str title = {tit, 1};
    str list[N_PAUSE] = {STR_RESUME, STR_RETRY, STR_HOME, STR_QUIT};
    int ind[N_PAUSE] = {PAUSE_RES_ID, PAUSE_RETR_ID, PAUSE_BACK_ID, PAUSE_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_PAUSE);
    
    int chosen = menu(title, set, gamevar);
    free(set.list);
    return chosen;
}

// Game Over Menu
int gameover_menu(int score, Game_t* gamevar) {
    char scorestr[LIM_STR_BUFF];
    sprintf(scorestr, "%s: %d", STR_SCORE, score); // Transform score int in str
    str tit[] = {STR_OVER, scorestr};
    List_str title = {tit, 2};
    str list[N_OVER] = {STR_RETRY, STR_HOME, STR_QUIT};
    int ind[N_OVER] = {OVER_RETR_ID, OVER_BACK_ID, OVER_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_OVER);

    int chosen = menu(title, set, gamevar); // Call menu
    free(set.list); // Free memory
    return chosen;
}

// Quit Menu
int quit_menu(const Game_t* gamevar) {
    str tit[] = {STR_QUIT_WARNING};
    List_str title = {tit, 1};
    str list[N_YN] = {STR_YES, STR_NO};
    int ind[N_OVER] = {YES_ID, NO_ID};
    List_str set = dict_to_list(list, ind, N_YN);

    int chosen = menu(title, set, gamevar); // Call menu
    free(set.list); // Free memory
    return chosen;
}

// Check if key is one of the first letters
void check_key(int key, int* hl, const List_str set) {
    if(key >= '1' && key <= '9' && key-'1' < set.len) {
        *hl = key-'1';
        return;
    }
    for(int i = mod(*hl+1, set.len); i != *hl; i = mod(i+1, set.len)) {
        if(key == set.list[i][0] || key == set.list[i][0]+CAPITAL_SHIFT) {
            *hl = i;
            return;
        }
    }
}
