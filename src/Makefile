CC=clang
CFLAGS=-g
LDFLAGS=-fsanitize=address

BUILD_DIR=./obj
BIN_DIR=./bin

OBJS=

all: dirs build

build: graph.o
	${CC} ${CFLAGS} main.c ${OBJS} -o ${BIN_DIR}/sudoku

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

