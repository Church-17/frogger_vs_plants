CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

version:
	make -C ../ VER=versione_processi

process: process.c process.h
	${COBJ}
