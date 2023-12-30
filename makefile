CC = gcc
CARGS_OBJ = -o $@ -fdiagnostics-color=always -Wall
CARGS = ${CARGS_OBJ} -lncursesw -lpthread -lSDL2 -lSDL2_mixer
COBJ = ${CC} -c $*.c ${CARGS_OBJ}
COBJ_VER = ${CC} -c ./${VERSION}/$*.c ${CARGS_OBJ}

# Libs dependences
struct = struct.h struct_proto.h
utils = utils.h struct_proto.h
res = res.h struct_proto.h
str = str.h ${res}
music = music.h
game = game.h ${res}
menu = menu.h struct_proto.h
main = main.h struct_proto.h
sprites = sprites.h ${game}
manche = manche.h ${sprites}
version = version.h struct_proto.h
entity = entity.h

# Define compiling version
VERSION = versione_processi

sopr_proj.out: main.o menu.o sprites.o music.o str.o res.o utils.o game.o manche.o entity.o version.o
	${CC} $^ ${CARGS}

version.o: ${VERSION}/version.c ${utils} ${struct} ${version}
	${COBJ_VER}

manche.o: manche.c ${main} ${menu} ${manche} ${music} ${utils} ${struct} ${version} ${entity}
	${COBJ}

entity.o: entity.c ${manche} ${utils} ${struct} ${version} ${entity}
	${COBJ}

main.o: main.c ${main} ${menu} ${game} ${music} ${str} ${utils} ${struct}
	${COBJ}

menu.o: menu.c ${main} ${menu} ${game} ${music} ${str} ${utils} ${struct}
	${COBJ}

game.o: game.c ${main} ${menu} ${manche} ${music} ${utils} ${struct}
	${COBJ}

sprites.o: sprites.c ${main} ${manche} ${str} ${utils} ${struct}
	${COBJ}

str.o: str.c ${str} ${utils}
	${COBJ}

res.o: res.c ${res} ${utils} ${struct}
	${COBJ}

utils.o: utils.c ${utils} ${struct}
	${COBJ}

music.o: music.c ${music}
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