// Include libs
#include <stdlib.h>
#include <curses.h>

// Define constant
#define ENTER 10

// Define macros
#define mod(x, y) ((x%y + y) % y)

// Define type and struct
typedef struct {
    int y;
    int x;
} Point;