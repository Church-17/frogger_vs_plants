#include <unistd.h>
#include <locale.h>
#include <ncurses.h>
#include <string.h>

int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    cbreak(); // Don't wait ENTER in getch()
    keypad(stdscr, TRUE); // Enable function keys listener

    system("mode con: cols=50 lines=15");

    getch();
    endwin();

}