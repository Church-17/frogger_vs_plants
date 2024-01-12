// Define sounds effects
#define SOUND_DEAD_PLANT 0
#define SOUND_BULLET 1
#define SOUND_MANCHE_LOST 2
#define SOUND_SCORE 3
#define SOUND_MENU_SELECTION 4
#define SOUND_SPAWN_PLANT 5
#define SOUND_START_GAME 6
#define SOUND_MANCHE_WON 7
#define N_SOUND_EFFECTS 8

// Define musics
#define MUSIC_BEST_SCORE 0
#define MUSIC_EASY 1
#define MUSIC_HARD 2
#define MUSIC_GAME_LOST 3
#define MUSIC_MEDIUM 4
#define MUSIC_MENU 5
#define MUSIC_GAME_WON 6
#define N_MUSICS 7

// Sound duration
#define SOUND_SCORE_DURATION 1100

// Function prototypes
void play_sound(int sound_id);
void play_music(int music_id);
void stop_music(void);
void pause_music(void);
void resume_music(void);