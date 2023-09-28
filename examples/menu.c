#include <ncurses.h>
#include <string.h>

#define N_OPTIONS 4
#define SUBMIT 10

int main(){

    initscr();
    curs_set(0);

    WINDOW* menuWin = newwin(N_OPTIONS*1.5, 30, 0, 0);
    box(menuWin, 0, 0);
    refresh();
    wrefresh(menuWin);

    keypad(menuWin, true);

    char* options[N_OPTIONS] = {"Restart", "Game settings", "Quit", "Cretits"};

    int choiche;
    int highlight = 0;

    while(1){

        for (int i = 0; i < N_OPTIONS; i++){    // dispone le opzioni evidenziandone una a seconda di highlight

            if (highlight == i)
                wattron(menuWin, A_REVERSE);
            mvwprintw(menuWin, i+1, 1, options[i]);
            wattroff(menuWin, A_REVERSE);
        }

        choiche = wgetch(menuWin);  // appena si riceve un carattere o si esce dal while o si torna al for superiore
                                    // (dopo che, nello switch, highlight è stato aggiornato)

        switch (choiche)
        {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = N_OPTIONS-1;
            break;
        
        case KEY_DOWN:
            highlight++;
            if (highlight == N_OPTIONS)
                highlight = 0;
            break;

        default:
            break;
        }

        if (choiche == SUBMIT)  // quando si preme ENTER si esce dal ciclo while
            break;
    
    }

    printw("Your choiche is %s", options[highlight]);

    getch();
    endwin();

    return 0;
}