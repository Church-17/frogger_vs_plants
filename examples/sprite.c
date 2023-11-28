#include <unistd.h>
#include <locale.h>
#include <ncurses.h>

#define mvaptrintw(y, x, attr, fstr...) attron(attr); mvprintw(y, x, fstr); attroff(attr)
#define GREEN_BLACK COLOR_PAIR(1)
#define BLACK_GREEN COLOR_PAIR(2)
#define YELLOW_BLACK COLOR_PAIR(3)
#define BLACK_YELLOW COLOR_PAIR(4)
#define YELLOW_GREEN COLOR_PAIR(5)
#define GREEN_YELLOW COLOR_PAIR(6)
#define MAGENTA_BLACK COLOR_PAIR(7)
#define MAGENTA_GREEN COLOR_PAIR(8)


int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    cbreak(); // Don't wait ENTER in getch()
    keypad(stdscr, TRUE); // Enable function keys listener

    start_color(); // Enable colors
    // Set each color with RGB values (scaled on thousandth)
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_WHITE, 800, 800, 800);
    init_color(COLOR_RED, 800, 0, 0);
    init_color(COLOR_GREEN, 0, 700, 200);
    init_color(COLOR_YELLOW, 800, 800, 0);
    init_color(COLOR_BLUE, 0, 300, 900);
    init_color(COLOR_MAGENTA, 600, 0, 600);
    init_color(COLOR_CYAN, 200, 600, 900);
    // Set needed color pairs (text, bg)
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_YELLOW, COLOR_GREEN);
    init_pair(6, COLOR_GREEN, COLOR_YELLOW);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(9, COLOR_BLACK, COLOR_WHITE);

    int y = 1, x = 1;

    mvaptrintw(y, x, GREEN_BLACK, "▄█"); //
    mvaptrintw(y, x+3, MAGENTA_GREEN, "▀");
    mvaptrintw(y, x+4, GREEN_YELLOW, "▌▐");
    mvaptrintw(y, x+6, MAGENTA_GREEN, "▀");
    mvaptrintw(y, x+8, GREEN_BLACK, "█▄"); //

    mvaptrintw(y+1, x+1, GREEN_BLACK, "▀▄"); //
    mvaptrintw(y+1, x+3, GREEN_YELLOW, " ▄  ");
    mvaptrintw(y+1, x+7, GREEN_BLACK, "▄▀"); //

    mvaptrintw(y+2, x+2, GREEN_BLACK, "▄"); //
    mvaptrintw(y+2, x+3, YELLOW_GREEN, " ▀▄▀");
    mvaptrintw(y+2, x+7, GREEN_BLACK, "▄"); //

    mvaptrintw(y+3, x, GREEN_BLACK, "▀█▀"); //
    mvaptrintw(y+3, x+7, GREEN_BLACK, "▀█▀"); //

    getch();

    printf("%d %d\n", LINES, COLS);
    endwin();

}