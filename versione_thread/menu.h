// Include libs
#include <ncurses.h>
#include "struct.h"

// Define constant
#define MIN_ROWS 15
#define MIN_COLS 50

// Prototypes
void home_menu(void);
void best_scores_menu(void);
void settings_menu(void);
void credits_menu(void);
int pause_menu(void);
int gameover_menu(int score);
bool check_term(int dim_y, int dim_x, int* act_LINES, int* act_COLS);