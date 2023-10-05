// Include libs
#include <curses.h>
#include <string.h>
#include "utils.h"
#include "menu.h"

int pause_menu() {
    str list[5] = {"Resume", "New game", "Settings", "Menu", "Quit"};
    List_str opts;
    opts.list = list;
    opts.len = 5;
    return menu("Pause", opts, TRUE);
}

int game() {


}