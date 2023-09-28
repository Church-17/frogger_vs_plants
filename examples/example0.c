#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define SPEED 3

typedef struct {
    int y;
    int x;
} Position;

int xBeg, yBeg, xMax, yMax;

void* mov(void* args);
void* shot(void* args);
void* raid(void* args);

int main(void){

    initscr();
    noecho();
    curs_set(0);

    pthread_t t1, t2, t3;

    getbegyx(stdscr, yBeg, xBeg);
    getmaxyx(stdscr, yMax, xMax);

    char* avatar = "<<-^->>";
    if (pthread_create(&t1, NULL, &mov, avatar))        // thread 1: spacesheep
        fprintf(stderr, "Failed on creating the thread\n");


    char* bullet = (char*) malloc(sizeof(char));
    *bullet = '$';
    if (pthread_create(&t2, NULL, &shot, bullet))      // thread 2: shot
        fprintf(stderr, "Failed on creating the thread\n");


    char* enemy = "\\^/";
    if (pthread_create(&t3, NULL, &raid, enemy))        // thread 3: enemies
        fprintf(stderr, "Failed on creating the thread\n");




    if (pthread_join(t1, NULL) != 0)
        fprintf(stderr, "Failed on closing the thread\n");

    if (pthread_join(t2, NULL) != 0)
        fprintf(stderr, "Failed on closing the thread\n");

    if (pthread_join(t3, NULL) != 0)
        fprintf(stderr, "Failed on closing the thread\n");

    getch();

    endwin();
    return 0;
}

void* mov(void* args){

    Position oggetto = {yMax/2, xMax/2};

    char* avatar = (char*) args;
    int ava_len = strlen(avatar);

    mvaddstr(oggetto.y, oggetto.x, avatar);

    while (true){

        int command = getch();
        switch (command)
        {
            case 'w':
                mvaddstr(oggetto.y, oggetto.x, "       ");
                oggetto.y -= SPEED;
                if (oggetto.y < 0)
                    oggetto.y = 0;
                break;

            case 'a':
                mvaddstr(oggetto.y, oggetto.x, "       ");
                oggetto.x -= SPEED;
                if (oggetto.x < 0)
                    oggetto.x = 0;

                mvaddstr(oggetto.y, oggetto.x, avatar);
                break;

            case 's':
                mvaddstr(oggetto.y, oggetto.x, "       ");
                oggetto.y += SPEED;
                if (oggetto.y == yMax)
                    oggetto.y = yMax-ava_len;
                break;

            case 'd':
                mvaddstr(oggetto.y, oggetto.x, "       ");
                oggetto.x += SPEED;
                if (oggetto.x  == xMax)
                    oggetto.x = xMax-ava_len;
                break;

            case 'q':
                endwin();
                exit(0);
                break;

            default:
                break;
        }

        mvaddstr(oggetto.y, oggetto.x, avatar);
        refresh();
    }

}

void* shot(void* args){

    char bullet = *(char*) args;

    Position bul_pos;

    int command;

    while(true){
        
        command = getch();
        if (command == 'e'){

            bul_pos.x = getcurx(stdscr)-4;
            bul_pos.y = getcury(stdscr)-1;

            mvaddch(bul_pos.y, bul_pos.x, bullet);
            refresh();

            for (int i = bul_pos.y-1; i > 0; i--){
                usleep(4000);
                mvaddch(i, bul_pos.x, bullet);
                refresh();
                mvaddch(i+1, bul_pos.x, ' ');
                refresh();
            }

        }
    }

}

void* raid(void* args){

    char* sprite = (char*) args;

    Position enemy;
    enemy.y = 1;

    while (true){

        enemy.x = rand()%xMax + xBeg;
        mvaddstr(enemy.y, enemy.x, sprite);
        refresh();
        sleep(1);


    }

}
