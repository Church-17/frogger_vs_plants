// Include libs
#include "utils.h"

// Define constants
#define BOX_PADN 2 // North padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADS 1 // South padding of the box
#define CREDITS " Credits "
#define PAUSE " Pause "
#define N_OPTS_HOME 5
#define N_OPTS_CREDS 4
#define N_OPTS_PAUSE 5

// Prototypes

int menu(str title, List_str opts, bool nav);

void best_scores(void);

void game_settings(void);

void credits_menu(void);

int pause_menu(void);

void endgame_menu(int score);