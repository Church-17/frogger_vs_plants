// Include libs
#include "struct.h"

// Define constants
#define BOX_DIM 2
#define BOX_PADN 2 // North padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADS 1 // South padding of the box
#define PAD_HL 1 // Highlight padding
#define LR_ARROWS 4

// Prototypes
void view(str title, List_str sx, List_str dx);
str menu(str title, List_str opts);
void settings(void);
void home_menu(void);
void best_scores(void);
void credits(void);
int pause(void);
void endgame(int score);