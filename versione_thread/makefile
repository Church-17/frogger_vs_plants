CC = gcc
CARGS = -fdiagnostics-color=always -Wall -lncursesw -lpthread

sopr_proj: main.o menu.o game.o resources.o utils.o
	${CC} $^ -o $@ ${CARGS}

main.o: game.h menu.h utils.h main.c
	${CC} -c $*.c -o $@ ${CARGS}

menu.o: menu.h game.h resources.h utils.h menu.c
	${CC} -c $*.c -o $@ ${CARGS}

game.o: game.h menu.h resources.h utils.h game.c
	${CC} -c $*.c -o $@ ${CARGS}

resources.o: resources.h utils.h resources.c
	${CC} -c $*.c -o $@ ${CARGS}

utils.o: utils.h utils.c
	${CC} -c $*.c -o $@ ${CARGS}

clean:
	rm *.o