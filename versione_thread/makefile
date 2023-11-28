CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

sopr_proj.out: ../main.o ../menu.o game.o ../res.o ../utils.o
	${CC} $^ ${CARGS}
	rm *.o
	rm ../*.o

game.o: game.c ../main.h ../menu.h ../game.h ../res.h ../utils.h
	${COBJ}

../main.o:
	make -C ../