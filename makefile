CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

VER = versione_processi
COBJ_VER = ${CC} -c ${VER}/$*.c ${CARGS}

sopr_proj.out: main.o menu.o game.o str.o res.o utils.o
	${CC} $^ ${CARGS}

main.o: main.c main.h menu.h game.h str.h res.h utils.h struct_proto.h
	${COBJ}

menu.o: menu.c main.h menu.h game.h str.h res.h utils.h struct.h struct_proto.h
	${COBJ}

game.o: ${VER}/game.c main.h menu.h game.h str.h res.h utils.h struct.h struct_proto.h
	${COBJ_VER}

str.o: str.c str.h res.h struct.h struct_proto.h
	${COBJ}

res.o: res.c res.h utils.h struct.h struct_proto.h
	${COBJ}

utils.o: utils.c utils.h struct.h struct_proto.h
	${COBJ}


# Clear routines

clear_all: clear_o clear_out

clear_o:
	rm *.o

clear_out:
	rm *.out