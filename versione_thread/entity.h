// Include libs
#include "game.h"

// Inter-object variables
extern bool frog_on_croccodile[MIN_PLANT_ID - MIN_CROCCODILE_ID], croccodile_shotted[MIN_PLANT_ID - MIN_CROCCODILE_ID];

// Function prototypes
void* time_thread(void* params);
void* frog_thread(void* params);
void* croccodile_thread(void* params);
void* plant_thread(void* params);
void* bullet_thread(void* params);