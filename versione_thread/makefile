CC = gcc
CARGS = -fdiagnostics-color=always -Wall -lncursesw -lpthread

sopr_proj: main.o menu.o game.o utils.o
	${CC} main.o menu.o game.o utils.o -o sopr_proj ${CARGS}

main.o: game.h menu.h utils.h main.c
	${CC} -c main.c -o main.o ${CARGS}

menu.o: menu.h game.h utils.h menu.c
	${CC} -c menu.c -o menu.o ${CARGS}

game.o: game.h menu.h utils.h game.c
	${CC} -c game.c -o game.o ${CARGS}

utils.o: utils.h utils.c
	${CC} -c utils.c -o utils.o ${CARGS}

clean:
	rm *.o