// Include libs
#include "struct.h"

// Define constants
#define TITLE " PAC-MAN "
#define CREDITS " Credits "
#define PAUSE " Pause "
#define BOX_PADN 2 // North padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADS 1 // South padding of the box
#define LR_ARROWS 4
#define N_OPTS_HOME 5
#define N_OPTS_CREDS 4
#define N_OPTS_PAUSE 5

// Prototypes
int menu(str title, List_str opts, bool nav);
int* double_menu(str title, List_str* opts, int* setted, int opts_len, bool nav);
void home_menu(void);
void best_scores(void);
void settings(void);
void credits_menu(void);
int pause_menu(void);
void endgame_menu(int score);