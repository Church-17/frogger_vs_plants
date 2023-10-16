#include "struct.h"

extern int game_params[];   
extern str strContainer[][2];   // extern allows to load a global variable from another object file

// Home
#define TITLE "PAC-MAN"
#define NEW_GAME strContainer[0][game_params[0]]
#define BEST_SCORES strContainer[1][game_params[0]]
#define SETTINGS strContainer[2][game_params[0]]
#define CREDITS strContainer[3][game_params[0]]
#define QUIT strContainer[4][game_params[0]]
#define N_HOME_SET 5

// Settings
#define LANGUAGE strContainer[5][game_params[0]]
#define DIFFICULTY strContainer[6][game_params[0]]
#define SKIN "Skin"
#define APPLY "Ok"
#define CANCEL strContainer[7][game_params[0]]
#define N_SETTINGS_SET 3
#define N_SETTINGS_SEL 2
// - Language
#define ITALIAN "Italiano"
#define ENGLISH "English"
#define N_LANG 2
// - Difficulty
#define DIFFICULTY_0 strContainer[8][game_params[0]]
#define DIFFICULTY_1 strContainer[9][game_params[0]]
#define DIFFICULTY_2 strContainer[10][game_params[0]]
#define N_DIFFICULTY 3
// - Skin
#define SKIN_0 strContainer[11][game_params[0]]
#define SKIN_1 strContainer[12][game_params[0]]
#define SKIN_2 strContainer[13][game_params[0]]
#define N_SKIN 3

// Credits
#define PROJECT strContainer[14][game_params[0]]
#define FRANCESCO "Francesco Cardia"
#define MATTEO "Matteo Chiesa"
#define N_CREDITS_SET 4

// Pause
#define PAUSE strContainer[15][game_params[0]]
#define RESUME strContainer[16][game_params[0]]
#define HOME strContainer[17][game_params[0]]
#define N_PAUSE_SET 5