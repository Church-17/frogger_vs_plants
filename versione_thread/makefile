CC = gcc
CARGS = -fdiagnostics-color=always -Wall -lncursesw -lpthread

sopr_proj: main.o menu.o game.o res.o utils.o
	${CC} $^ -o $@ ${CARGS}

main.o: menu.h res.h main.c
	${CC} -c $*.c -o $@ ${CARGS}

menu.o: menu.h game.h res.h utils.h menu.c
	${CC} -c $*.c -o $@ ${CARGS}

game.o: game.h menu.h res.h utils.h game.c
	${CC} -c $*.c -o $@ ${CARGS}

res.o: res.h utils.h res.c
	${CC} -c $*.c -o $@ ${CARGS}

utils.o: utils.h utils.c
	${CC} -c $*.c -o $@ ${CARGS}

clean:
	rm *.o