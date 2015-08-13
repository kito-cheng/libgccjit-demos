PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CXX=g++
CXXFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: plus1

clean:
	rm -rf plus1 plus1.o

plus1: plus1.o
	$(CXX) -o $@ $< $(LDFLAGS)

plus1.o: plus1.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)
