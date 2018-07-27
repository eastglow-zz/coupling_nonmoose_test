coupling_nonmoose_test
=====

"Fork coupling_nonmoose_test" to create a new MOOSE-based application.

For more information see: [http://mooseframework.org/create-an-app/](http://mooseframework.org/create-an-app/)

This MOOSE application calls a function from the external library which can be loaded dynamically.

The function in the external library prints out integers from 1 to 10.\
The function is built in a static library; ./my_external_app/static/lib/libfunc.arcv.a\
With the wrapper code that enables a function in the static library be loaded dynamically, a shared library is built; ./my_external_app/static/lib/wrapper.so\
The wrapper code and corresponding header files are in ./my_external_app/static/src and ./my_external_app/static/include, respectively.\
The functions is called in MOOSE TimeStepper, hence the function is called at every MOOSE timestep.\
The function loading and calling routine is wriitten in MyTimeStepper::stet() function in  ./src/timesteppers/MyTimeStepper.C\



