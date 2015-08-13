PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CXX=g++
CXXFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib

all: helloworld

clean:
	rm -rf helloworld helloworld.o

helloworld: helloworld.o
	$(CXX) -o $@ $< $(LDFLAGS)

helloworld.o: helloworld.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)
