PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: brainfuck

clean:
	rm -rf brainfuck brainfuck.o

brainfuck: brainfuck.o
	$(CC) -o $@ $(LDFLAGS) $<

brainfuck.o: brainfuck.c
	$(CC) -o $@ -c $< $(CFLAGS)
