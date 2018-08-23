
headerpath=$(PWD)/include
srcpath=$(PWD)/src
libpath=$(PWD)/mylibs
CXX=g++

#Wrpper dynamic library linked with the static library
wrapper.so: $(srcpath)/wrapper.cpp $(libpath)/print_one_to_ten.so
	$(CXX) -shared -fPIC -o $(libpath)/wrapper.so $(srcpath)/wrapper.cpp $(libpath)/print_one_to_ten.so -I$(headerpath)

$(libpath)/print_one_to_ten.so: $(srcpath)/print_one_to_ten.cpp
	$(CXX) -shared -fPIC -o $(libpath)/print_one_to_ten.so $(srcpath)/print_one_to_ten.cpp

clean:
	rm -f myextapp $(libpath)/wrapper.so $(libpath)/print_one_to_ten.so ./*.o

.PHONY: clean
