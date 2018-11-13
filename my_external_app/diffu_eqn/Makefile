
headerpath=$(PWD)/include
srcpath=$(PWD)/src
libpath=$(PWD)/mylib

#Wrpper dynamic library linked with the static library
wrapper_diffu_eqn_ser.so: $(srcpath)/wrapper_diffu_eqn_ser.cpp $(libpath)/libdiffu_fncs.a
	$(CXX) $(CXXFLAGS) -shared -o $(libpath)/wrapper_diffu_eqn_ser.so $(srcpath)/wrapper_diffu_eqn_ser.cpp $(libpath)/libdiffu_fncs.a -I$(headerpath)

#Compiling the static library containing the functions to use
$(libpath)/libdiffu_fncs.a: diffu_eqn_ser.o
	ar rcv $(libpath)/libdiffu_fncs.a diffu_eqn_ser.o

diffu_eqn_ser.o: $(srcpath)/diffu_eqn_ser.cpp
	$(CXX) $(CXXFLAGS) -c $(srcpath)/diffu_eqn_ser.cpp -I$(headerpath)

clean:
	rm -f $(libpath)/*.so $(libpath)/*.a ./*.o

.PHONY: clean
