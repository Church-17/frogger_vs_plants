// Include libs
#include <ncurses.h>
#include "struct_proto.h"

// Function prototypes
void rd_settings(void);
void wr_settings(int* set);
Dict_str_int rd_best(void);
void wr_best(Dict_str_int best);

// --- ID OPTIONS ---
// Home
#define HOME_GAME_ID 0
#define HOME_BEST_ID 1
#define HOME_SETT_ID 2
#define HOME_CRED_ID 3
#define HOME_QUIT_ID 4
#define N_HOME 5
// Best
#define N_BEST 3
// Settings
#define SET_LANG_ID 0
#define SET_DIFF_ID 1
#define SET_SKIN_ID 2
#define SET_COL1_ID 3
#define SET_COL2_ID 4
#define SET_VOL_MUS_ID 5
#define SET_VOL_EFCT_ID 6
#define N_SETTINGS 7
#define SET_APPL_ID (N_SETTINGS+0)
#define SET_CANC_ID (N_SETTINGS+1)
#define N_SET_SEL (N_SETTINGS+2)
#define LANG_SET game_settings[SET_LANG_ID]
#define DIFF_SET game_settings[SET_DIFF_ID]
#define SKIN_SET game_settings[SET_SKIN_ID]
#define COL1_SET game_settings[SET_COL1_ID]
#define COL2_SET game_settings[SET_COL2_ID]
#define VOL_MUS_SET game_settings[SET_VOL_MUS_ID]
#define VOL_EFCT_SET game_settings[SET_VOL_EFCT_ID]
// - Language
#define LANG_EN_ID 0
#define LANG_IT_ID 1
#define N_LANGUAGE 2
// - Difficulty
#define DIFF_0_ID 0
#define DIFF_1_ID 1
#define DIFF_2_ID 2
#define N_DIFFICULTY 3
// - Skin
#define SKIN_0_ID 0
#define SKIN_1_ID 1
#define N_SKIN 2
// - Color
#define N_COLOR 9
// - Volume
#define N_VOLUME 11
// Credits
#define N_CREDITS 2
// Pause
#define PAUSE_RES_ID 0
#define PAUSE_RETR_ID 1
#define PAUSE_BACK_ID 2
#define PAUSE_QUIT_ID 3
#define N_PAUSE 4
// Gameover
#define OVER_RETR_ID 0
#define OVER_BACK_ID 1
#define OVER_QUIT_ID 2
#define N_OVER 3
// Warning quit
#define YES_ID 0
#define NO_ID 1
#define N_YN 2

// --- INTER-OBJECT VARS ---
extern int game_settings[N_SETTINGS]; // Game settings

// --- COLORS ---
// Color ID
#define COLOR_GREY 8
#define COLOR_ORANGE 9
#define COLOR_PURPLE 10
#define COLOR_BORDEAUX 11
#define COLOR_DARKGREEN 12
#define COLOR_DARKBLUE 13
#define COLOR_LIGHTGREEN 14
#define COLOR_PINK 15
// Color pair ID
#define WHITE_BLACK_ID 0 // FIXED with WHITE_BLACK
#define RED_BLACK_ID 1
#define ORANGE_BLACK_ID 2
#define YELLOW_BLACK_ID 3
#define GREEN_BLACK_ID 4
#define CYAN_BLACK_ID 5
#define BLUE_BLACK_ID 6
#define MAGENTA_BLACK_ID 7
#define GREY_BLACK_ID 8
#define GREEN_YELLOW_ID 9
#define MAGENTA_GREEN_ID 10
#define GREEN_PURPLE_ID 11
#define GREEN_DARKBLUE_ID 12
#define GREEN_BORDEAUX_ID 13
#define GREEN_DARKGREEN_ID 14
#define BORDEAUX_DARKBLUE_ID 15
#define DARKGREEN_DARKBLUE_ID 16
#define LIGHTGREEN_DARKGREEN_ID 17
#define WHITE_DARKGREEN_ID 18
#define CYAN_DARKBLUE_ID 19
#define WHITE_BORDEAUX_ID 20
#define PINK_BORDEAUX_ID 21
#define RED_PURPLE_ID 22
#define RED_DARKBLUE_ID 23
#define RED_DARKGREEN_ID 24
#define RED_BORDEAUX_ID 25
#define RED_ORANGE_ID 26
#define CYAN_RED_ID 27
#define DARKGREEN_PURPLE_ID 28
#define WHITE_RED_ID 29
#define WHITE_PURPLE_ID 30
#define GREEN_GREY_ID 31
#define RED_GREY_ID 32
// Color pair
#define WHITE_BLACK COLOR_PAIR(WHITE_BLACK_ID)
#define RED_BLACK COLOR_PAIR(RED_BLACK_ID)
#define ORANGE_BLACK COLOR_PAIR(ORANGE_BLACK_ID)
#define YELLOW_BLACK COLOR_PAIR(YELLOW_BLACK_ID)
#define GREEN_BLACK COLOR_PAIR(GREEN_BLACK_ID)
#define GREY_BLACK COLOR_PAIR(GREY_BLACK_ID)
#define GREEN_YELLOW COLOR_PAIR(GREEN_YELLOW_ID)
#define MAGENTA_GREEN COLOR_PAIR(MAGENTA_GREEN_ID)
#define GREEN_PURPLE COLOR_PAIR(GREEN_PURPLE_ID)
#define GREEN_DARKBLUE COLOR_PAIR(GREEN_DARKBLUE_ID)
#define GREEN_DARKGREEN COLOR_PAIR(GREEN_DARKGREEN_ID)
#define GREEN_BORDEAUX COLOR_PAIR(GREEN_BORDEAUX_ID)
#define BORDEAUX_DARKBLUE COLOR_PAIR(BORDEAUX_DARKBLUE_ID)
#define DARKGREEN_DARKBLUE COLOR_PAIR(DARKGREEN_DARKBLUE_ID)
#define LIGHTGREEN_DARKGREEN COLOR_PAIR(LIGHTGREEN_DARKGREEN_ID)
#define WHITE_DARKGREEN COLOR_PAIR(WHITE_DARKGREEN_ID)
#define CYAN_DARKBLUE COLOR_PAIR(CYAN_DARKBLUE_ID)
#define WHITE_BORDEAUX COLOR_PAIR(WHITE_BORDEAUX_ID)
#define PINK_BORDEAUX COLOR_PAIR(PINK_BORDEAUX_ID)
#define RED_PURPLE COLOR_PAIR(RED_PURPLE_ID)
#define RED_DARKBLUE COLOR_PAIR(RED_DARKBLUE_ID)
#define RED_DARKGREEN COLOR_PAIR(RED_DARKGREEN_ID)
#define RED_BORDEAUX COLOR_PAIR(RED_BORDEAUX_ID)
#define RED_ORANGE COLOR_PAIR(RED_ORANGE_ID)
#define CYAN_RED COLOR_PAIR(CYAN_RED_ID)
#define DARKGREEN_PURPLE COLOR_PAIR(DARKGREEN_PURPLE_ID)
#define WHITE_RED COLOR_PAIR(WHITE_RED_ID)
#define WHITE_PURPLE COLOR_PAIR(WHITE_PURPLE_ID)
#define GREEN_GREY COLOR_PAIR(GREEN_GREY_ID)
#define RED_GREY COLOR_PAIR(RED_GREY_ID)
// Color pair setted in settings
#define COL1 COLOR_PAIR(COL1_SET)
#define COL2 COLOR_PAIR(COL2_SET)
