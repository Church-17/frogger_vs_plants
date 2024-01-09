// Inter-object variables
extern int frog_on_croccodile;

// Function prototypes
void* time_thread(void* params);
void* frog_thread(void* params);
void* croccodile_thread(void* params);
void* plant_thread(void* params);
void* bullet_thread(void* params);
void change_croccodile_shotted(int id);