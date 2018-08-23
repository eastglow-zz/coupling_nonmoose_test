# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.
CXX=g++
example1: main.cpp hello.so
	$(CXX) $(CXXFLAGS) -o example1 main.cpp -ldl

hello.so: hello.cpp
#	$(CXX) $(CXXFLAGS) -shared -o hello.so hello.cpp
	$(CXX) $(CXXFLAGS) -fPIC -nostartfiles -o hello.so hello.cpp

clean:
	rm -f example1 hello.so

.PHONY: clean
