CC = gcc
CARGS_OBJ = -o $@ -fdiagnostics-color=always -Wall
CARGS = ${CARGS_OBJ} -lncursesw -lpthread
COBJ = ${CC} -c $*.c ${CARGS_OBJ}

# Libs dependences
struct_proto = struct_proto.h
struct = struct.h ${struct_proto}
utils = utils.h ${struct_proto}
res = res.h ${struct_proto}
str = str.h ${res}
music = music.h
game = game.h ${res}
menu = menu.h ${struct_proto}
main = main.h ${struct_proto}
sprites = sprites.h ${game}
manche = manche.h ${sprites}
thread = thread.h ${struct_proto}
entity = entity.h ${game}

frogger_vs_plants.out: main.o menu.o sprites.o music.o str.o res.o utils.o game.o manche.o entity.o thread.o
	${CC} $^ ${CARGS}

manche.o: manche.c ${main} ${menu} ${manche} ${music} ${utils} ${struct} ${entity} ${thread}
	${COBJ}

thread.o: thread.c ${utils} ${struct} ${thread}
	${COBJ}

entity.o: entity.c ${manche} ${utils} ${struct} ${entity} ${thread}
	${COBJ}

main.o: main.c ${main} ${menu} ${game} ${music} ${str} ${utils} ${struct}
	${COBJ}

menu.o: menu.c ${main} ${menu} ${game} ${music} ${str} ${utils} ${struct}
	${COBJ}

game.o: game.c ${main} ${menu} ${manche} ${music} ${utils} ${struct}
	${COBJ}

sprites.o: sprites.c ${main} ${manche} ${str} ${utils} ${struct}
	${COBJ}

str.o: str.c ${res} ${utils}
	${COBJ}

res.o: res.c ${res} ${utils} ${struct}
	${COBJ}

utils.o: utils.c ${utils} ${struct}
	${COBJ}

music.o: music.c ${music} ${res} ${utils} ${struct_proto}
	${COBJ}

# Install dependencies
install:
	sudo apt install mpg123

# Clear routine
clear:
	rm -f *.o
	rm -f *.out
