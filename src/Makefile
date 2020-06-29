CC=clang
CFLAGS=-g
LDFLAGS=-fsanitize=address

BUILD_DIR=./obj
BIN_DIR=./bin

MCFLAGS=${CFLAGS} `pkg-config --cflags gtk+-3.0`
MLDFLAGS=${LDFLAGS} `pkg-config --libs gtk+-3.0`

OBJS=

all: dirs build

build: graph.o
	${CC} ${MCFLAGS} sudoku.c ${OBJS} -o ${BIN_DIR}/sudoku ${MLDFLAGS}

tests:

run:
	${BIN_DIR}/sudoku

dirs:
	mkdir ${BUILD_DIR} || true
	mkdir ${BIN_DIR} || true
clean:
	rm -rf ${BUILD_DIR} || true
	rm -rf ${BIN_DIR} || true

#BUILD COMMANDS HERE

graph.o:
	${CC} ${CFLAGS} -c graph.c -o ${BUILD_DIR}/graph.o ${LDFLAGS}

