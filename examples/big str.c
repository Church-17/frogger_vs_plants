#include <unistd.h>
#include <locale.h>
#include <ncurses.h>
#include <string.h>

typedef char* str;

void print_str(int y, int x, str string);

int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    cbreak(); // Don't wait ENTER in getch()
    keypad(stdscr, TRUE); // Enable function keys listener

    print_str(0, 0, "Ciao");

    getch();
    endwin();

}

void print_str(int y, int x, str string) {
    int len = strlen(string);
    for(int i = 0; i < len; i++) {
        switch(string[i]) {
            case 'a':
        }
    }
}