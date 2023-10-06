CC = gcc
CARGS = -Wall -lncursesw -lpthread

sopr_proj: main.o menu.o game.o utils.o:
	${CC} $^ -o sopr_proj ${CARGS}

main.o: utils.h game.h menu.h main.c
	${CC} main.c -o main.o ${CARGS}

menu.o: utils.h game.h menu.c
	${CC} menu.c -o main.o ${CARGS}

game.o: utils.h menu.h game.c
	${CC} game.c -o main.o ${CARGS}

clean:
	rm *.o