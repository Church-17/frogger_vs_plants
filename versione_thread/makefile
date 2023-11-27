CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ_COMM = ${CC} -c ../$*.c ${CARGS}
COBJ_SPEC = ${CC} -c $*.c ${CARGS}

sopr_proj.out: main.o menu.o game.o res.o utils.o
	${CC} $^ ${CARGS}
	rm *.o

main.o: ../main.c ../main.h ../menu.h ../game.h ../res.h ../utils.h
	${COBJ_COMM}

menu.o: ../menu.c ../main.h ../menu.h ../game.h ../res.h ../utils.h
	${COBJ_COMM}

game.o: game.c ../main.h ../menu.h ../game.h ../res.h ../utils.h
	${COBJ_SPEC}

res.o: ../res.c ../res.h ../utils.h
	${COBJ_COMM}

utils.o: ../utils.c ../utils.h
	${COBJ_COMM}