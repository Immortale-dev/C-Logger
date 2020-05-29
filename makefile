.PHONY: all generate_o generate_t dist

CC=g++
CFLAGS=-c -Wall -x c++
LDFLAGS=
SRCPATH:=src/
SRCS:=$(wildcard $(SRCPATH)*.cpp)
OBJS:=$(SRCS:%.cpp=%.o)
INCL=-Isrc -Itest


all: generate_o generate_t

	
generate_o: ${OBJS}

generate_t: 
	${CC} ${CFLAGS} ${INCL} -o test/test.o test/test.cpp
	${CC} ${INCL} -o test.exe test/test.o ${OBJS}
	
dist: generate_o

%.o: %.cpp
	${CC} ${CFLAGS} ${INCL} $< -o $@
	
	
