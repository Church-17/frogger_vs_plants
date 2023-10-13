// Include libs
#include "struct.h"

// Define constants
#define BOX_PADN 2 // North padding of the box
#define BOX_PADE 5 // East padding of the box
#define BOX_PADW 2 // West padding of the box
#define BOX_PADS 1 // South padding of the box
#define HL_PADX 1 // Highlight padding
#define SEL_PADY 1 // Empty lines in settings
#define LR_ARROWS 4 // N chars occupied by arrows in settings

// Prototypes
void view(str title, List_str sx, List_str dx);
int menu(str title, List_str opts);
void settings(void);
void home_menu(void);
void best_scores(void);
void credits(void);
int pause(void);
void endgame(int score);