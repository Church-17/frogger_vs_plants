CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

VER = versione_thread
COBJ_VER = ${CC} -c ${VER}/$*.c ${CARGS}

sopr_proj.out: main.o menu.o game.o res.o utils.o
	${CC} $^ ${CARGS}

main.o: main.c main.h menu.h game.h res.h utils.h
	${COBJ}

menu.o: menu.c main.h menu.h game.h res.h utils.h
	${COBJ}

game.o: ${VER}/game.c main.h menu.h game.h res.h utils.h
	${COBJ_VER}

res.o: res.c res.h utils.h
	${COBJ}

utils.o: utils.c utils.h
	${COBJ}

clear_all: clean_o clean_out

clear_o:
	rm *.o

clear_out:
	rm *.out