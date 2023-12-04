CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

COMMON = ../main.o ../menu.o ../sprites.o ../str.o ../res.o ../utils.o ../game.o

.PHONY: sopr_proj.out
sopr_proj.out: manche.o process.o frog.o time.o
	make common -C ../
	${CC} $^ ${COMMON} ${CARGS}

manche.o: manche.c ../main.h ../menu.h ../manche.h ../sprites.h ../str.h ../res.h ../utils.h ../struct.h ../struct_proto.h
	${COBJ}

process.o: process.c process.h ../utils.h ../struct.h ../struct_proto.h
	${COBJ}

frog.o: frog.c frog.h process.h ../utils.h ../struct.h ../struct_proto.h
	${COBJ}

time.o: time.c time.h process.h ../utils.h ../struct.h ../struct_proto.h
	${COBJ}