// Include prototypes of structs
#include <sys/wait.h>
#include <pthread.h>
#include <ncurses.h>
#include "struct_proto.h"

// Define structs
struct List_str {
    str* list;
    int len;
};
struct Dict_str_int {
    str* key;
    int* val;
    int len;
};
struct List_attr {
    attr_t* list;
    int len;
};
struct List_pid {
    pid_t* list;
    int len;
};
struct List_thread {
    pthread_t* list;
    int len;
};
struct Message {
    int id;
    int y;
    int x;
    int sig;
};
struct Position {
    int y;
    int x;
};
struct Game_t {
    bool* holes_occupied;
    int timer;
    int score;
    int lifes;
    int win;
    int frog_on_croccodile;
    int free_frog_bullet;
    int* stream_speed;
    Position frog;
    Position* frog_bullets;
    Position* plants;
    Position** plants_bullets;
    Position** croccodiles;
    int** croccodiles_kind;
};
