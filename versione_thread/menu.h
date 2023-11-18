// Include libs
#include "struct.h"

// Prototypes
WINDOW* init_menu(str title, int rows, int cols);
void view(str title, List_str sx, List_str dx, List_attr cols);
int menu(str title, List_str opts);
void home_menu(void);
void best_scores_menu(void);
void settings_menu(void);
void credits_menu(void);
int pause_menu(void);
void endgame_menu(int score);