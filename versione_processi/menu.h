// Include libs
#include "struct_proto.h"

// Prototypes
int home_menu(void);
void best_scores_menu(void);
void settings_menu(void);
void credits_menu(void);
int pause_menu(const Game_t* gamevar);
int gameover_menu(int score, Game_t* gamevar);
int quit_menu(const Game_t* gamevar);
str insert_name(const Game_t* gamevar);