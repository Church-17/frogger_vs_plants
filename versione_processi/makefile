CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

COMMON = ../main.o ../menu.o ../sprites.o ../str.o ../res.o ../utils.o

sopr_proj.out: game.o process.o
	make common -C ../
	${CC} $^ ${COMMON} ${CARGS}

game.o: game.c ../main.h ../menu.h ../game.h ../str.h ../res.h ../utils.h ../struct.h ../struct_proto.h
	${COBJ}

process.o: process.c process.h ../utils.h ../struct.h ../struct_proto.h
	${COBJ}