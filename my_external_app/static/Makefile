
headerpath=$(PWD)/include
srcpath=$(PWD)/src
libpath=$(PWD)/lib

#Wrpper dynamic library linked with the static library
wrapper.so: $(srcpath)/wrapper.cpp $(libpath)/libfunc_arcv.a
	$(CXX) $(CXXFLAGS) -shared -o $(libpath)/wrapper.so $(srcpath)/wrapper.cpp $(libpath)/libfunc_arcv.a -I$(headerpath)

#Compiling the static library containing the functions to use
$(libpath)/libfunc_arcv.a: print_one_to_ten.o
	ar rcv $(libpath)/libfunc_arcv.a print_one_to_ten.o

print_one_to_ten.o: $(srcpath)/print_one_to_ten.cpp
	$(CXX) $(CXXFLAGS) -c $(srcpath)/print_one_to_ten.cpp

clean:
	rm -f myextapp $(libpath)/wrapper.so $(libpath)/libfunc_arcv.a ./*.o

.PHONY: clean
