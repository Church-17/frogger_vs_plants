#include <unistd.h>
#include <locale.h>
#include <ncurses.h>

#define mvaptrintw(y, x, attr, fstr...) attron(attr); mvprintw(y, x, fstr); attroff(attr)
#define GREEN_BLACK COLOR_PAIR(1)
#define GREEN_YELLOW COLOR_PAIR(2)
#define MAGENTA_GREEN COLOR_PAIR(3)
#define YELLOW_BALCK COLOR_PAIR(4)
#define GREEN_WHITE COLOR_PAIR(5)
#define GREEN_D_GREEN COLOR_PAIR(6)
#define ORANGE_WHITE COLOR_PAIR(7)
#define ORANGE_D_ORANGE COLOR_PAIR(8)
#define ORANGE_BLACK COLOR_PAIR(9)
#define BLUE_BLACK COLOR_PAIR(10)
#define RED_BLACK COLOR_PAIR(11)
#define RED_ORANGE COLOR_PAIR(12)
#define CYAN_RED COLOR_PAIR(13)
#define RED_YELLOW COLOR_PAIR(14)

#define COLOR_D_GREEN 10
#define COLOR_ORANGE 11
#define COLOR_D_ORANGE 12

int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize windowd
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
    init_color(COLOR_D_GREEN, 124, 256, 236);
    init_color(COLOR_ORANGE, 900, 600, 100);
    init_color(COLOR_D_ORANGE, 455, 208, 100);
    // Set needed color pairs (text, bg)
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_YELLOW);
    init_pair(3, COLOR_MAGENTA, COLOR_GREEN);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_WHITE);
    init_pair(6, COLOR_GREEN, COLOR_D_GREEN);
    init_pair(7, COLOR_ORANGE, COLOR_WHITE);
    init_pair(8, COLOR_ORANGE, COLOR_D_ORANGE);
    init_pair(9, COLOR_ORANGE, COLOR_BLACK);
    init_pair(10, COLOR_BLUE, COLOR_BLACK);
    init_pair(11, COLOR_RED, COLOR_BLACK);
    init_pair(12, COLOR_RED, COLOR_ORANGE);
    init_pair(13, COLOR_CYAN, COLOR_RED);
    init_pair(14, COLOR_RED, COLOR_YELLOW);

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
    mvaptrintw(y+2, x+3, GREEN_YELLOW, "█▄▀▄");
    mvaptrintw(y+2, x+7, GREEN_BLACK, "▄"); //

    mvaptrintw(y+3, x, GREEN_BLACK, "▀█▀"); //
    mvaptrintw(y+3, x+7, GREEN_BLACK, "▀█▀"); //


    y = 1, x = 15;

    mvaptrintw(y, x, MAGENTA_GREEN, " "); //
    mvaptrintw(y, x+2, MAGENTA_GREEN, "▀");
    mvaptrintw(y, x+3, GREEN_YELLOW, "▌▐");
    mvaptrintw(y, x+5, MAGENTA_GREEN, "▀");
    mvaptrintw(y, x+7, MAGENTA_GREEN, " "); //

    mvaptrintw(y+1, x+1, GREEN_BLACK, "▀"); //
    mvaptrintw(y+1, x+2, GREEN_YELLOW, "▄▀▄ ");
    mvaptrintw(y+1, x+6, GREEN_BLACK, "▀"); //

    mvaptrintw(y+2, x, GREEN_BLACK, "▄█▀▀");
    mvaptrintw(y+2, x+4, YELLOW_BALCK, "▀"); //
    mvaptrintw(y+2, x+5, GREEN_BLACK, "▀█▄");

    y = 1, x = 25;
    mvaptrintw(y, x+6, GREEN_WHITE, "▀");
    mvaptrintw(y, x+7, GREEN_BLACK, "██████▄▄▄▄▄▄▄");

    mvaptrintw(y+1, x, GREEN_BLACK, "▄█");
    mvaptrintw(y+1, x+2, GREEN_D_GREEN, "▀");
    mvaptrintw(y+1, x+3, GREEN_WHITE, "▀");
    mvaptrintw(y+1, x+4, GREEN_D_GREEN, "▀");
    mvaptrintw(y+1, x+5, GREEN_WHITE, "▀");
    mvaptrintw(y+1, x+6, GREEN_D_GREEN, "▀");
    mvaptrintw(y+1, x+7, GREEN_WHITE, "▀");
    mvaptrintw(y+1, x+8, GREEN_D_GREEN, "▀");
    mvaptrintw(y+1, x+9, GREEN_WHITE, "▀");
    mvaptrintw(y+1, x+10, GREEN_BLACK, "██████████████████▄▄");

    mvaptrintw(y+2, x, GREEN_BLACK, "██");
    mvaptrintw(y+2, x+2, GREEN_WHITE, "▄");
    mvaptrintw(y+2, x+3, GREEN_D_GREEN, "▄");
    mvaptrintw(y+2, x+4, GREEN_WHITE, "▄");
    mvaptrintw(y+2, x+5, GREEN_D_GREEN, "▄");
    mvaptrintw(y+2, x+6, GREEN_WHITE, "▄");
    mvaptrintw(y+2, x+7, GREEN_D_GREEN, "▄");
    mvaptrintw(y+2, x+8, GREEN_WHITE, "▄");
    mvaptrintw(y+2, x+9, GREEN_D_GREEN, "▄");
    mvaptrintw(y+2, x+10, GREEN_BLACK, "█████████████████████");

    mvaptrintw(y+3, x+10, GREEN_BLACK, "▀ ▀     ▀ ▀");


    y = 1, x = 65;
    mvaptrintw(y, x+6, ORANGE_WHITE, "▀");
    mvaptrintw(y, x+7, ORANGE_BLACK, "██████▄▄▄▄▄▄▄");

    mvaptrintw(y+1, x, ORANGE_BLACK, "▄█");
    mvaptrintw(y+1, x+2, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+3, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+4, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+5, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+6, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+7, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+8, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+9, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+10, ORANGE_BLACK, "██████████████████▄▄");

    mvaptrintw(y+2, x, ORANGE_BLACK, "██");
    mvaptrintw(y+2, x+2, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+3, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+4, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+5, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+6, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+7, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+8, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+9, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+10, ORANGE_BLACK, "█████████████████████");

    mvaptrintw(y+3, x+10, ORANGE_BLACK, "▀ ▀     ▀ ▀");


    y = 1, x = 100;
    mvaptrintw(y, x+6, ORANGE_WHITE, "▀");
    mvaptrintw(y, x+7, ORANGE_BLACK, "██████▄▄▄▄▄▄▄");
    mvaptrintw(y, x, BLUE_BLACK, "⬤");

    mvaptrintw(y+1, x, ORANGE_BLACK, "▄█");
    mvaptrintw(y+1, x+2, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+3, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+4, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+5, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+6, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+7, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+8, ORANGE_D_ORANGE, "▀");
    mvaptrintw(y+1, x+9, ORANGE_WHITE, "▀");
    mvaptrintw(y+1, x+10, ORANGE_BLACK, "██████████████████▄▄");

    mvaptrintw(y+2, x, ORANGE_BLACK, "██");
    mvaptrintw(y+2, x+2, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+3, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+4, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+5, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+6, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+7, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+8, ORANGE_WHITE, "▄");
    mvaptrintw(y+2, x+9, ORANGE_D_ORANGE, "▄");
    mvaptrintw(y+2, x+10, ORANGE_BLACK, "█████████████████████");

    mvaptrintw(y+3, x+10, ORANGE_BLACK, "▀ ▀     ▀ ▀");


    
    y = 7, x = 1;
    mvaptrintw(y, x, RED_BLACK, "▄█"); //
    mvaptrintw(y, x+3, CYAN_RED, "▀");
    mvaptrintw(y, x+4, RED_ORANGE, "▌▐");
    mvaptrintw(y, x+6, CYAN_RED, "▀");
    mvaptrintw(y, x+8, RED_BLACK, "█▄"); //

    mvaptrintw(y+1, x+1, RED_BLACK, "▀▄"); //
    mvaptrintw(y+1, x+3, RED_ORANGE, " ▄  ");
    mvaptrintw(y+1, x+7, RED_BLACK, "▄▀"); //

    mvaptrintw(y+2, x+2, RED_BLACK, "▄"); //
    mvaptrintw(y+2, x+3, RED_ORANGE, "█▄▀▄");
    mvaptrintw(y+2, x+7, RED_BLACK, "▄"); //

    mvaptrintw(y+3, x, RED_BLACK, "▀█▀"); //
    mvaptrintw(y+3, x+7, RED_BLACK, "▀█▀"); //


    while(getch() != 'q');

    endwin();

}