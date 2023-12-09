CC = gcc
CARGS = -o $@ -fdiagnostics-color=always -Wall -lncursesw -lpthread -lvlc
COBJ = ${CC} -c $*.c ${CARGS}
COBJ_VER = ${CC} -c ./${VERSION}/$*.c ${CARGS}

# Libs dependences
struct = struct.h struct_proto.h
utils = utils.h struct_proto.h
res = res.h struct_proto.h
str = str.h ${res}
game = game.h struct_proto.h
menu = menu.h struct_proto.h
main = main.h struct_proto.h
sprites = sprites.h ${main} ${game} struct_proto.h
manche = manche.h ${sprites}
process = ${VERSION}/process.h struct_proto.h
time = ${VERSION}/time.h struct_proto.h
frog = ${VERSION}/frog.h struct_proto.h
music = music.h struct_proto.h

# Define compiling version
VERSION = versione_processi
ifeq (${VERSION}, versione_processi)
SPEC_FILE = process.o
else
SPEC_FILE = threads.o
endif

sopr_proj.out: main.o menu.o sprites.o str.o res.o utils.o game.o manche.o frog.o time.o music.o ${SPEC_FILE}
	${CC} $^ ${CARGS}

manche.o: ${VERSION}/manche.c ${process} ${time} ${frog} ${menu} ${manche} ${res} ${utils} ${struct}
	${COBJ_VER}

process.o: ${VERSION}/process.c ${process} ${utils} ${struct}
	${COBJ_VER}

frog.o: ${VERSION}/frog.c ${frog} ${process} ${main} ${game} ${struct}
	${COBJ_VER}

time.o: ${VERSION}/time.c ${time} ${process} ${utils} ${struct}
	${COBJ_VER}

main.o: main.c ${main} ${menu} ${game} ${str} ${utils} ${struct}
	${COBJ}

menu.o: menu.c ${main} ${menu} ${game} ${str} ${utils} ${struct} ${music}
	${COBJ}

game.o: game.c ${menu} ${manche} ${res} ${utils} ${struct}
	${COBJ}

sprites.o: sprites.c ${sprites} ${res} ${utils} ${struct}
	${COBJ}

str.o: str.c ${str} ${utils}
	${COBJ}

res.o: res.c ${res} ${utils} ${struct}
	${COBJ}

utils.o: utils.c ${utils} ${struct}
	${COBJ}

music.o: ${music}
	${COBJ}

# Clear routines

clear_all: clear_obj clear_out

clear_obj:
	rm -f *.o
	rm -f versione_processi/*.o
	rm -f versione_thread/*.o

clear_out:
	rm -f *.out
	rm -f versione_processi/*.out
	rm -f versione_thread/*.out