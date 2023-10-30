// Include libs
#include "struct.h"

// Inter-object variables
extern int game_settings[];
#define LANG_SET game_settings[0]
#define SKIN_SET game_settings[1]
#define DIFF_SET game_settings[2]
#define COL1_SET game_settings[3]
#define COL2_SET game_settings[4]
extern str strContainer[][2];

// Function prototypes
void rd_settings(void);
void wr_settings(void);
List_UserScore rd_best(void);
void wr_best(List_UserScore best);

// --- COLORS ---
#define COLS1 COLOR_PAIR(COL1_SET)
#define COLS2 COLOR_PAIR(COL2_SET)
#define YELLOW_BLACK COLOR_PAIR(3)
#define GREY_BLACK COLOR_PAIR(7)
#define BROWN_BLACK COLOR_PAIR(8)

// --- STRINGS ---

// Home
#define TITLE "PAC-MAN"
#define NEW_GAME strContainer[0][LANG_SET]
#define BEST_SCORES strContainer[1][LANG_SET]
#define SETTINGS strContainer[2][LANG_SET]
#define CREDITS strContainer[3][LANG_SET]
#define QUIT strContainer[4][LANG_SET]
#define N_HOME 5

// Best
#define N_BEST 3

// Settings
#define LANGUAGE strContainer[5][LANG_SET]
#define DIFFICULTY strContainer[6][LANG_SET]
#define FIRST_COLOR strContainer[18][LANG_SET]
#define SECOND_COLOR strContainer[19][LANG_SET]
#define SKIN "Skin"
#define APPLY "Ok"
#define CANCEL strContainer[7][LANG_SET]
#define N_SETTINGS 5
#define N_SETTINGS_SEL 2
// - Language
#define LANGUAGE_0 "English"
#define LANGUAGE_1 "Italiano"
#define N_LANGUAGE 2
// - Difficulty
#define DIFFICULTY_0 strContainer[8][LANG_SET]
#define DIFFICULTY_1 strContainer[9][LANG_SET]
#define DIFFICULTY_2 strContainer[10][LANG_SET]
#define N_DIFFICULTY 3
// - Skin
#define SKIN_0 strContainer[11][LANG_SET]
#define SKIN_1 strContainer[12][LANG_SET]
#define SKIN_2 strContainer[13][LANG_SET]
#define N_SKIN 3
// - Color
#define COLOR_0 strContainer[20][LANG_SET]
#define COLOR_1 strContainer[21][LANG_SET]
#define COLOR_2 strContainer[22][LANG_SET]
#define COLOR_3 strContainer[23][LANG_SET]
#define COLOR_4 strContainer[24][LANG_SET]
#define COLOR_5 "Magenta"
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
#define HOME strContainer[17][LANG_SET]
#define N_PAUSE 5

// Endgame