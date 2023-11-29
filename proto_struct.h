// Include libs
#include <ncurses.h>

// Define macros to typedef structs
#define typestructdef(datatype) struct datatype; typedef struct datatype datatype

// Define types & scruct prototypes
typedef char* str;
typestructdef(List_str);
typestructdef(List_attr);
typestructdef(Dict_str_int);