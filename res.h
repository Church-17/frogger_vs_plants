// Include libs
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
#define N_SETTINGS 5
#define SET_APPL_ID (N_SETTINGS+0)
#define SET_CANC_ID (N_SETTINGS+1)
#define N_SET_SEL (N_SETTINGS+2)
#define LANG_SET game_settings[SET_LANG_ID]
#define DIFF_SET game_settings[SET_DIFF_ID]
#define SKIN_SET game_settings[SET_SKIN_ID]
#define COL1_SET game_settings[SET_COL1_ID]
#define COL2_SET game_settings[SET_COL2_ID]
// - Language
#define LANG_0_ID 0
#define LANG_1_ID 1
#define N_LANGUAGE 2
// - Difficulty
#define DIFF_0_ID 0
#define DIFF_1_ID 1
#define DIFF_2_ID 2
#define N_DIFFICULTY 3
// - Skin
#define SKIN_0_ID 0
#define SKIN_1_ID 1
#define SKIN_2_ID 2
#define N_SKIN 3
// - Color
#define COL_0_ID 0
#define COL_1_ID 1
#define COL_2_ID 2
#define COL_3_ID 3
#define COL_4_ID 4
#define COL_5_ID 5
#define COL_6_ID 6
#define N_COLOR 7
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

// --- INTER-OBJECT VARS ---
extern int game_settings[N_SETTINGS]; // Game settings

// --- COLORS ---
#define COLOR_GREY 8
#define COLOR_BROWN 9
#define GREY_BLACK_ID 7
#define BROWN_BLACK_ID 8
#define COL1 COLOR_PAIR(COL1_SET)
#define COL2 COLOR_PAIR(COL2_SET)
#define YELLOW_BLACK COLOR_PAIR(COL_3_ID)
#define GREY_BLACK COLOR_PAIR(GREY_BLACK_ID)
#define BROWN_BLACK COLOR_PAIR(BROWN_BLACK_ID)
