PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: helloworld

clean:
	rm -rf helloworld helloworld.o

helloworld: helloworld.o
	$(CC) -o $@ $(LDFLAGS) $<

helloworld.o: helloworld.c
	$(CC) -o $@ -c $< $(CFLAGS)
