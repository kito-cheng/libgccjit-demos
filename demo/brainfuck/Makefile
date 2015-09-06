PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CC=gcc
CFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib
TARGETS=brainfuck brainfuck-dbg brainfuck-opt brainfuck-opt-dump

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)

%: %.o
	$(CC) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
