CC = gcc
CARGS = -fdiagnostics-color=always -Wall -lncursesw -lpthread

sopr_proj: main.o menu.o game.o utils.o strings.o
	${CC} $^ -o $@ ${CARGS}

main.o: game.h menu.h utils.h main.c
	${CC} -c $*.c -o $@ ${CARGS}

menu.o: menu.h game.h utils.h menu.c
	${CC} -c $*.c -o $@ ${CARGS}

game.o: game.h menu.h utils.h game.c
	${CC} -c $*.c -o $@ ${CARGS}

utils.o: utils.h utils.c
	${CC} -c $*.c -o $@ ${CARGS}

strings.o: utils.h strings.c
	${CC} -c $*.c -o $@ ${CARGS}

clean:
	rm *.o