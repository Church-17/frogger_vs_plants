// Include libs
#include <ncurses.h>

// Define macros to typedef structs
#define typestructdef(datatype) struct datatype; typedef struct datatype datatype

// Define types & scruct prototypes
typedef char* str;
typestructdef(List_int);
typestructdef(List_str);
typestructdef(Dict_str_int);
typestructdef(List_attr);
typedef List_int List_pid;
typestructdef(Message);
typestructdef(Position);
typestructdef(List_position);
typestructdef(Game_t);
