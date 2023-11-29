#include <unistd.h>
#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    setlocale(LC_ALL, ""); // Enable UTF-8 chars
    initscr(); // Initialize window
    noecho(); // Hide character typed
    curs_set(0); // Remove cursor
    cbreak(); // Don't wait ENTER in getch()
    keypad(stdscr, TRUE); // Enable function keys listener

    bool in_pause = FALSE, change = FALSE;
    int key;
    int pipe_fds[2];
    pid_t pid1, pid2;

    if(pipe(pipe_fds) == -1) {
        // Scrive su stderr, ma include la descrizione dell'ultimo errore che si è presentato
        perror("Pipe call");
        exit(1);
    }


    pid1 = fork();
    if(pid1 < 0) {
        perror("fork call");
        _exit(2);
    }  // Check fork a buon fine
    // Controllo: che processo sono?
    if(pid1 == 0) {  // Processo figlio
        // Scrivi i messaggi sul buffer della pipe
        while(TRUE) {
            close(pipe_fds[0]);
            write(pipe_fds[1], "z", 1);
            sleep(1);
        }
        _exit(0);
    }

    pid2 = fork();
    if(pid2 < 0) {
        perror("fork call");
        _exit(2);
    }  // Check fork a buon fine
    // Controllo: che processo sono?
    if(pid2 == 0) {  // Processo figlio
        // Scrivi i messaggi sul buffer della pipe
        while(TRUE) {
            key = getch();
            switch(key) {
                case 'p':
                    write(pipe_fds[1], "p", 1);
                    break;
                case 'q':
                    write(pipe_fds[1], "q", 1);
                    break;
            }
        }
        _exit(0);
    }

    char str[1];
    while(TRUE) {
        read(pipe_fds[0], str, 1);
        if(str[0] == 'z') {
            printw("z");
            refresh();
        } else if(str[0] == 'q') {
            kill(pid1, SIGKILL);
            endwin();
            exit(0);
        } else if (str[0] == 'p') {
            if(!in_pause) {
                kill(pid1, SIGSTOP);
                in_pause = TRUE;
            } else {
                kill(pid1, SIGCONT);
                in_pause = FALSE;
            }
        }
    }



}