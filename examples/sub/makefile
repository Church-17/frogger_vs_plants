CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

sopr_proj.out: subwin.o menu.o res.o utils.o
	${CC} $^ ${CARGS}
	rm *.o

subwin.o: subwin.c menu.h res.h utils.h
	${COBJ}

menu.o: menu.c menu.h res.h utils.h
	${COBJ}

res.o: res.c res.h utils.h
	${COBJ}

utils.o: utils.c utils.h
	${COBJ}