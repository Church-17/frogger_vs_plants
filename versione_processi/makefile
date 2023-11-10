CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -g -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

clean: sopr_proj.out
	rm *.o

sopr_proj.out: main.o menu.o game.o res.o utils.o
	${CC} $^ ${CARGS}

main.o: menu.h res.h utils.h main.c
	${COBJ}

menu.o: menu.h game.h res.h utils.h menu.c
	${COBJ}

game.o: game.h menu.h res.h utils.h game.c
	${COBJ}

res.o: res.h utils.h res.c
	${COBJ}

utils.o: utils.h utils.c
	${COBJ}