// Include prototypes of structs
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
struct Message {
    int id;
    int y;
    int x;
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
    int* stream_speed;
    Position frog;
    Position** croccodiles;
    bool** bad_croccodiles;
};
