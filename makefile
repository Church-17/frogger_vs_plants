CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

common: main.o menu.o res.o utils.o

main.o: main.c main.h menu.h game.h res.h utils.h
	${COBJ}

menu.o: menu.c main.h menu.h game.h res.h utils.h
	${COBJ}

res.o: res.c res.h utils.h
	${COBJ}

utils.o: utils.c utils.h
	${COBJ}

clear:
	rm *.o