# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.

#example1: main.cpp hello.so
#	$(CXX) $(CXXFLAGS) -o example1 main.cpp -ldl
headerpath=$(PWD)/include

mytryapp: clientapp.cpp wrapper.so
	$(CXX) $(CXXFLAGS) -o mytryapp clientapp.cpp -ldl

#hello.so: hello.cpp
#	$(CXX) $(CXXFLAGS) -shared -o hello.so hello.cpp

wrapper.so: wrapper.cpp libfunc_arcv.a $(headerpath)
	$(CXX) $(CXXFLAGS) -shared -o wrapper.so wrapper.cpp libfunc_arcv.a -I$(headerpath)

libfunc_arcv.a: func_arcv.o
	ar rcv libfunc_arcv.a func_arcv.o

func_arcv.o: func_arcv.cpp
	$(CXX) $(CXXFLAGS) -c func_arcv.cpp

clean:
#	rm -f example1 hello.so
	rm -f mytryapp wrapper.so libfunc_arcv.a

.PHONY: clean
.PHONY: all
all: ; $(info $$var is [${var}])echo Hello world
