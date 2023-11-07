// Include libs
#include "struct.h"

// Define constant
#define LIM_STR_BUFF 100

// Function prototypes
void rd_settings(void);
void wr_settings(void);
Dict_str_int rd_best(void);
void wr_best(Dict_str_int best);
Dict_str_int check_conf_file(FILE* fptr, int lines, int lim);

// --- SETTINGS ---
extern int game_settings[];

#define SET_LANG_ID 0
#define SET_DIFF_ID 1
#define SET_SKIN_ID 2
#define SET_COL1_ID 3
#define SET_COL2_ID 4
#define LANG_SET game_settings[SET_LANG_ID]
#define DIFF_SET game_settings[SET_DIFF_ID]
#define SKIN_SET game_settings[SET_SKIN_ID]
#define COL1_SET game_settings[SET_COL1_ID]
#define COL2_SET game_settings[SET_COL2_ID]

// --- STRINGS ---
extern str strContainer[][2];

// Home
#define TITLE "PAC-MAN"
#define HOME_NEWG_ID 0
#define NEW_GAME strContainer[0][LANG_SET]
#define HOME_BEST_ID 1
#define BEST_SCORES strContainer[1][LANG_SET]
#define HOME_SETT_ID 2
#define SETTINGS strContainer[2][LANG_SET]
#define HOME_CRED_ID 3
#define CREDITS strContainer[3][LANG_SET]
#define HOME_QUIT_ID 4
#define QUIT strContainer[4][LANG_SET]
#define N_HOME 5

// Best
#define N_BEST 3
#define NULL_USER "NULL"
#define NULL_RECORD 0

// Settings
#define LANGUAGE strContainer[5][LANG_SET]
#define DIFFICULTY strContainer[6][LANG_SET]
#define SKIN "Skin"
#define FIRST_COLOR strContainer[18][LANG_SET]
#define SECOND_COLOR strContainer[19][LANG_SET]
#define N_SETTINGS 5
#define SET_APPL_ID 0
#define APPLY "Ok"
#define SET_CANC_ID 1
#define CANCEL strContainer[7][LANG_SET]
#define N_SETTINGS_SEL 2
// - Language
#define LANG_0_ID 0
#define LANGUAGE_0 "English"
#define LANG_1_ID 1
#define LANGUAGE_1 "Italiano"
#define N_LANGUAGE 2
// - Difficulty
#define DIFF_0_ID 0
#define DIFFICULTY_0 strContainer[8][LANG_SET]
#define DIFF_1_ID 1
#define DIFFICULTY_1 strContainer[9][LANG_SET]
#define DIFF_2_ID 2
#define DIFFICULTY_2 strContainer[10][LANG_SET]
#define N_DIFFICULTY 3
// - Skin
#define SKIN_0_ID 0
#define SKIN_0 strContainer[11][LANG_SET]
#define SKIN_1_ID 1
#define SKIN_1 strContainer[12][LANG_SET]
#define SKIN_2_ID 2
#define SKIN_2 strContainer[13][LANG_SET]
#define N_SKIN 3
// - Color
#define COL_0_ID 0
#define COLOR_0 strContainer[20][LANG_SET]
#define COL_1_ID 1
#define COLOR_1 strContainer[21][LANG_SET]
#define COL_2_ID 2
#define COLOR_2 strContainer[22][LANG_SET]
#define COL_3_ID 3
#define COLOR_3 strContainer[23][LANG_SET]
#define COL_4_ID 4
#define COLOR_4 strContainer[24][LANG_SET]
#define COL_5_ID 5
#define COLOR_5 "Magenta"
#define COL_6_ID 6
#define COLOR_6 strContainer[25][LANG_SET]
#define N_COLOR 7

// Credits
#define PROJECT strContainer[14][LANG_SET]
#define FRANCESCO "Francesco Cardia"
#define MATTEO "Matteo Chiesa"
#define N_CREDITS 4

// Pause
#define PAUSE strContainer[15][LANG_SET]
#define RESUME strContainer[16][LANG_SET]
#define BACK_HOME strContainer[17][LANG_SET]
#define N_PAUSE 5

// Endgame

// --- COLORS ---
#define GREY_BLACK_ID 7
#define BROWN_BLACK_ID 8
#define COLS1 COLOR_PAIR(COL1_SET)
#define COLS2 COLOR_PAIR(COL2_SET)
#define YELLOW_BLACK COLOR_PAIR(COL_3_ID)
#define GREY_BLACK COLOR_PAIR(GREY_BLACK_ID)
#define BROWN_BLACK COLOR_PAIR(BROWN_BLACK_ID)