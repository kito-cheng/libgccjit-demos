all: install/lib/libgccjit.so

clean:

distclean:
	rm -rf build install

PWD:=$(shell pwd)
NPROC=$(shell cat /proc/cpuinfo | grep processor |wc -l)

install/lib/libgccjit.so: build/gcc/Makefile
	cd build/gcc/ && make all -j$(NPROC)
	cd build/gcc/ && make install -j$(NPROC)

build/gcc/Makefile: srcs/.gcc.stamp
	mkdir -p $(dir $@)
	cd $(dir $@) && \
	  ../../srcs/gcc-5.2.0/configure \
	    --enable-host-shared \
	    --enable-languages=jit \
	    --disable-bootstrap \
	    --prefix=$(PWD)/install \
	    --disable-multilib \
	    --disable-nls

download/gcc-5.2.0.tar.bz2:
	mkdir -p $(dir $@)
	cd $(dir $@) && \
	  wget https://ftp.gnu.org/gnu/gcc/gcc-5.2.0/gcc-5.2.0.tar.bz2

srcs/.gcc.stamp: download/gcc-5.2.0.tar.bz2
	mkdir -p $(dir $@)
	cd $(dir $@) && tar -jxf ../$<
	touch $@
