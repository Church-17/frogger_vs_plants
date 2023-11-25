// Include libs
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

// Function prototypes
int menu(List_str title, List_str opts);
bool resize_proc(WINDOW* win, int dim_y, int dim_x);
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr);
void check_key(int key, int* hl, List_str* set);

extern WINDOW* main_scr;

// General function for a single column menu, returning index of selected option
int menu(List_str title, List_str set) {
    // Init vars
    bool do_prints, do_return = FALSE; // Flags
    int i, key, inc, old_hl = 0, hl = 0;
    int win_width = WIN_WIDTH(max_strlen(set, 0), 0, max_strlen(title, 0)) + HL_PADX; // Calc window width
    int win_height = POSITION_Y(set.len, set.len+1, title.len)+BOX_PADS; // Calc window height

    // Setup window
    WINDOW* menu_win = subwin(main_scr, win_height, win_width, (LINES - win_height)/2, (COLS - win_width)/2); // Centered window
    keypad(menu_win, TRUE);
    wattron(menu_win, COLS1);

    while(!do_return) {
        // Prints
        box(menu_win, ACS_VLINE, ACS_HLINE); // Box window
        for(i = 0; i < title.len; i++) { // Print title
            wctrprintw(menu_win, TITLE_ROW+i, " %s ", title.list[i]);
        }
        for(i = 0; i < set.len; i++) { // Print options *** with first letter underlined
            mvwfattrprintw(menu_win, POSITION_Y(i, set.len, title.len), BOX_PADX, A_UNDERLINE, set.list[i]);
        }
        do_prints = FALSE;
        
        while(!do_prints) {
            // Update non-highlighted & highlighted option
            redrawwin(menu_win);
            mvwfattrprintw(menu_win, POSITION_Y(old_hl, set.len, title.len), BOX_PADX, A_UNDERLINE, set.list[old_hl]);
            wprintw(menu_win, "%*s", HL_PADX, "");
            mvwprintw(menu_win, POSITION_Y(hl, set.len, title.len), BOX_PADX, "%*s", HL_PADX, ""); // Print hl padding
            wattroff(menu_win, COLS1);
            wattron(menu_win, A_STANDOUT | COLS2);
            mvwprintw(menu_win, POSITION_Y(hl, set.len, title.len), BOX_PADX+HL_PADX, "%s", set.list[hl]);
            wattroff(menu_win, A_STANDOUT | COLS2);
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
                    do_prints = TRUE;
                    do_return = TRUE;
                    break;

                case KEY_RESIZE:
                    do_prints = resize_proc(main_scr, 15, 50);
                    break;

                default:
                    // Check numbers & first letter
                    check_key(key, &hl, &set);
                    break;
            }
        }
    }
    unwin(menu_win);
    return hl;
}

// Home Menu
int home_menu(void) {
    // Init vars
    str tit[] = {TITLE};
    List_str title;
    title.list = tit;
    title.len = 1;
    str list[N_HOME] = {NEW_GAME, BEST_SCORES, SETTINGS, CREDITS, QUIT};
    int ind[N_HOME] = {HOME_GAME_ID, HOME_BEST_ID, HOME_SETT_ID, HOME_CRED_ID, HOME_QUIT_ID};
    List_str set = dict_to_list(list, ind, N_HOME);

    int chosen = menu(title, set); // Call menu
    
    free(set.list); // Free memory

    return chosen;
}

// Check if term is large enough
bool check_term(WINDOW* win) {
    if(LINES < 15 || COLS < 50) {
        if(win != NULL) {
            mv_win(win, 0, 0);
        }
        WINDOW* err_win = new_win(0, 0, 0, 0);
        wattron(err_win, COLS1);
        mvwprintw(err_win, 0, 0, "%s", EXTEND);
        mvwprintw(err_win, 1, 0, "%s: %d x %d    ", MINIMUM, 15, 50);
        while(LINES < 15 || COLS < 50) {
            mvwprintw(err_win, 2, 0, "%s: %d x %d    ", ACTUAL, LINES, COLS);
            wgetch(err_win);
        }
        unwin(err_win);
        return TRUE;
    }
    return FALSE;
}

// Resize procedure
bool resize_proc(WINDOW* win, int dim_y, int dim_x) {
    bool do_prints = check_term(win);
    if(win->_maxy >= dim_y) win->_maxy = dim_y-1; // Fix: don't stick window on Y-axis
    if(win->_maxx >= dim_x) win->_maxx = dim_x-1; // Fix: don't stick window on X-axis
    mv_win(win, (LINES - win->_maxy)/2, (COLS - win->_maxx)/2);
    // clear();
    wrefresh(win);
    return do_prints;
}

// Move & print string with first letter attributed
void mvwfattrprintw(WINDOW* win, int row, int col, attr_t attr, str fstr) {
    wattron(win, A_UNDERLINE);
    mvwprintw(win, row, col, "%c", fstr[0]);
    wattroff(win, A_UNDERLINE);
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
