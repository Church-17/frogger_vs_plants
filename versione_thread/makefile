CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

sopr_proj.out: main.o menu.o game.o res.o utils.o
	${CC} $^ ${CARGS}
	rm *.o

main.o: main.c menu.h res.h utils.h
	${COBJ}

menu.o: menu.c menu.h game.h res.h utils.h
	${COBJ}

game.o: game.c game.h menu.h res.h utils.h
	${COBJ}

res.o: res.c res.h utils.h
	${COBJ}

utils.o: utils.c utils.h
	${COBJ}