# Getting start

## Build libgccjit !

### Pre-Requirement for build gcc

Ubuntu:
    sudo apt-get install libgmp-dev libmpc-dev libmpfr-dev libcloog-isl-dev cloog-isl zlib1g-dev build-essential texinfo

### Build libgccjit
    make

Yes, just make and wait, and then you can see a libgccjit.so in install/lib

    $ ls install/lib/libgccjit.so
    install/lib/libgccjit.so

## Try Demo!

### Brainf*ck

Build brainf*ck!

    $ cd demo/brainfuck
    $ make

Run with Hello World!

    $ PATH=`pwd`/../../install/bin:$PATH ./brainfuck tests/hello.bf

Note: libgccjit.so will invoke gcc driver in background so you need to add the install path into the $PATH
