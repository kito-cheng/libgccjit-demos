PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: braninfuck

clean:
	rm -rf braninfuck braninfuck.o

braninfuck: braninfuck.o
	$(CC) -o $@ $(LDFLAGS) $<

braninfuck.o: braninfuck.c
	$(CC) -o $@ -c $< $(CFLAGS)
