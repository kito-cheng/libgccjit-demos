PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: empty

clean:
	rm -rf empty empty.o

empty: empty.o
	$(CC) -o $@ $(LDFLAGS) $<

empty.o: empty.c
	$(CC) -o $@ -c $< $(CFLAGS)
