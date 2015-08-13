PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CXX=g++
CXXFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: empty

clean:
	rm -rf empty empty.o

empty: empty.o
	$(CXX) -o $@ $< $(LDFLAGS)

empty.o: empty.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)
