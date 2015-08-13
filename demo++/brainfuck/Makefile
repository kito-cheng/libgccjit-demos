PWD:=$(shell pwd)
LIBGCCJIT_BASEPATH=$(PWD)/../../install
CXX=g++
CXXFLAGS=-I$(LIBGCCJIT_BASEPATH)/include -g3
LDFLAGS=-L$(LIBGCCJIT_BASEPATH)/lib -lgccjit -Wl,-rpath,$(LIBGCCJIT_BASEPATH)/lib
TARGETS=brainfuck brainfuck-dbg brainfuck-opt brainfuck-opt-dump

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)

brainfuck: brainfuck.o
	$(CXX) -o $@ $< $(LDFLAGS)

brainfuck.o: brainfuck.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)

brainfuck-dbg: brainfuck-dbg.o
	$(CXX) -o $@ $< $(LDFLAGS)

brainfuck-dbg.o: brainfuck-dbg.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)

brainfuck-opt: brainfuck-opt.o
	$(CXX) -o $@ $< $(LDFLAGS)

brainfuck-opt.o: brainfuck-opt.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)

brainfuck-opt-dump: brainfuck-opt-dump.o
	$(CXX) -o $@ $< $(LDFLAGS)

brainfuck-opt-dump.o: brainfuck-opt-dump.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS)
