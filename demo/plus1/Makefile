PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: plus1

clean:
	rm -rf plus1 plus1.o

plus1: plus1.o
	$(CC) -o $@ $(LDFLAGS) $<

plus1.o: plus1.c
	$(CC) -o $@ -c $< $(CFLAGS)
