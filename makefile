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
process = versione_processi/process.h struct_proto.h
time = ${VERSION}/time.h struct_proto.h
frog = ${VERSION}/frog.h struct_proto.h
croccodile = ${VERSION}/croccodile.h ${res}
plant = ${VERSION}/plant.h struct_proto.h
bullet = ${VERSION}/bullet.h struct_proto.h

# Define compiling version
VERSION = versione_processi
ifeq (${VERSION}, versione_processi)
SPEC_OBJ = process.o
SPEC_LIB = ${process}
else
SPEC_OBJ = thread.o
SPEC_LIB = ${threads}
endif

sopr_proj.out: main.o menu.o sprites.o music.o str.o res.o utils.o game.o manche.o frog.o time.o croccodile.o plant.o bullet.o ${SPEC_OBJ}
	${CC} $^ ${CARGS}

manche.o: ${VERSION}/manche.c ${main} ${menu} ${manche} ${utils} ${struct} ${SPEC_LIB} ${time} ${frog} ${croccodile}
	${COBJ_VER}

process.o: versione_processi/process.c ${utils} ${struct} ${process}
	${COBJ_VER}

frog.o: ${VERSION}/frog.c ${main} ${game} ${utils} ${struct} ${SPEC_LIB} ${frog}
	${COBJ_VER}

time.o: ${VERSION}/time.c ${game} ${utils} ${struct} ${SPEC_LIB} ${time}
	${COBJ_VER}

croccodile.o: ${VERSION}/croccodile.c ${sprites} ${utils} ${struct} ${SPEC_LIB} ${croccodile}
	${COBJ_VER}

plant.o: ${VERSION}/plant.c ${sprites} ${utils} ${struct} ${SPEC_LIB} ${plant}
	${COBJ_VER}

bullet.o: ${VERSION}/bullet.c ${sprites} ${utils} ${struct} ${SPEC_LIB} ${bullet}
	${COBJ_VER}

main.o: main.c ${main} ${menu} ${game} ${music} ${str} ${utils} ${struct}
	${COBJ}

menu.o: menu.c ${main} ${menu} ${game} ${music} ${str} ${utils} ${struct}
	${COBJ}

game.o: game.c ${main} ${menu} ${manche} ${utils} ${struct}
	${COBJ}

sprites.o: sprites.c ${main} ${sprites} ${str} ${utils} ${struct} ${manche}
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