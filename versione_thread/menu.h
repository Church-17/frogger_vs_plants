// Include libs
#include "struct.h"
#include "strings.h"

// Define constants
#define BOX_DIM 2
#define BOX_PADN 2 // North padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADS 1 // South padding of the box
#define PAD_HL 1 // Highlight padding
#define LR_ARROWS 4

// Prototypes
int menu(str title, List_str opts, bool nav);
int* double_menu(str title, List_str sets, List_str* opts, int* setted, bool nav);
void home_menu(void);
void best_scores(void);
void settings(void);
void credits_menu(void);
int pause_menu(void);
void endgame_menu(int score);