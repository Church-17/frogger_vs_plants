CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS}

default:
	make -C ./versione_processi/

common: main.o menu.o sprites.o str.o res.o utils.o game.o

main.o: main.c main.h menu.h game.h str.h res.h utils.h struct_proto.h
	${COBJ}

menu.o: menu.c main.h menu.h game.h str.h res.h utils.h struct.h struct_proto.h
	${COBJ}

game.o: game.c main.h menu.h game.h str.h res.h utils.h struct.h struct_proto.h
	${COBJ}

sprites.o: sprites.c sprites.h res.h utils.h struct_proto.h
	${COBJ}

str.o: str.c str.h res.h struct.h struct_proto.h
	${COBJ}

res.o: res.c res.h utils.h struct.h struct_proto.h
	${COBJ}

utils.o: utils.c utils.h struct.h struct_proto.h
	${COBJ}

# Clear routines

clear_all: clear_o clear_out

clear_o:
	rm -f *.o
	rm -f versione_processi/*.o
	rm -f versione_thread/*.o

clear_out:
	rm -f *.out
	rm -f versione_processi/*.out
	rm -f versione_thread/*.out