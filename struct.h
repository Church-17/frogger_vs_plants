// Include prototypes of structs
#include "struct_proto.h"

// Define structs
struct List_int {
    int* list;
    int len;
};
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
    Position frog;
    int timer;
    int* lifes;
    bool* holes_occupied;
};
