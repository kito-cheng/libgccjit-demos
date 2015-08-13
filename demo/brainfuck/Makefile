PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib
TARGETS=brainfuck brainfuck-dbg brainfuck-opt brainfuck-opt-dump

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)

brainfuck: brainfuck.o
	$(CC) -o $@ $< $(LDFLAGS)

brainfuck.o: brainfuck.c
	$(CC) -o $@ -c $< $(CFLAGS)

brainfuck-dbg: brainfuck-dbg.o
	$(CC) -o $@ $< $(LDFLAGS)

brainfuck-dbg.o: brainfuck-dbg.c
	$(CC) -o $@ -c $< $(CFLAGS)

brainfuck-opt: brainfuck-opt.o
	$(CC) -o $@ $< $(LDFLAGS)

brainfuck-opt.o: brainfuck-opt.c
	$(CC) -o $@ -c $< $(CFLAGS)

brainfuck-opt-dump: brainfuck-opt-dump.o
	$(CC) -o $@ $< $(LDFLAGS)

brainfuck-opt-dump.o: brainfuck-opt-dump.c
	$(CC) -o $@ -c $< $(CFLAGS)
