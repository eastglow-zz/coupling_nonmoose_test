#include "MyDiffuTimeStepper.h"
#include <diffu_eqn_ser.h>
#include <dlfcn.h>

template<>
InputParameters validParams<MyDiffuTimeStepper>()
{
  InputParameters params = validParams<TimeStepper>();
  params.addParam<Real>("dt", 1.0, "Size of the time step");
  params.addParam<std::string>("library_path_name",
                               "default",
                               "Name with the path for the dynamic library to load");
  return params;
}

MyDiffuTimeStepper::MyDiffuTimeStepper(const InputParameters & parameters) :
    TimeStepper(parameters),
    _dt(getParam<Real>("dt")),
    _ext_lib_path_name(getParam<std::string>("library_path_name"))
{
}

/* This method is a pure virtual function, so it must be redefined by any
daughter class, even if the behavior doesn't change. */
Real
MyDiffuTimeStepper::computeInitialDT()
{
  return _dt;
}

/* This method is a pure virtual function, so it must be redefined by any
daughter class, even if the behavior doesn't change. */
Real
MyDiffuTimeStepper::computeDT()
{
  return _dt;
}

void
MyDiffuTimeStepper::step()
{
  ////////////////
  /*
  printf("Executing internal custum routine\n");
  for (unsigned int i = 1; i <= 10; i++)
  {
    printf("%d\n",i);
  }
  */
  ////////////////
  using std::cout;
  using std::cerr;
  printf("Calling external functions\n");
  //void* handle = dlopen("/Users/donguk.kim/projects/coupling_nonmoose_test/external_app/static/lib/wrapper.so", RTLD_LAZY);
  void* handle = dlopen(_ext_lib_path_name.c_str(), RTLD_LAZY);
  //the library path above alse can be provided by the input file

  if (!handle) {
    cerr << "Cannot open library: " << dlerror() << '\n';
  }

  dlerror();
  //Function loadings; calc_x(), data_set_value(), output_data_console(), and Diffu_module()

  // typedef void* (*fp1)(double*, int, int, int, double, double);
  // fp1 mdp_calc_x;
  // typedef void* (*fp2)(double*, int, int, double);
  // fp2 mdp_data_set_value;
  // typedef void* (*fp3)(double*, int, int, double*, double);
  // fp3 mdp_output_data_console;
  // typedef void* (*fp4)(double*, double*, DiffuParams);
  // fp4 mdp_Diffu_module;
  void (*mdp_calc_x)(double*, int, int, int, double, double)= (void (*)(double*, int, int, int, double, double)) dlsym(handle, "wrapper_calc_x");
  const char *dlsym_error1 = dlerror();

  void (*mdp_data_set_value)(double*, int, int, double) =(void (*)(double*, int, int, double)) dlsym(handle, "wrapper_data_set_value");
  const char *dlsym_error2 = dlerror();

  void (*mdp_output_data_console)(double*, int, int, double*, double) = (void (*)(double*, int, int, double*, double)) dlsym(handle, "wrapper_output_data_console");
  const char *dlsym_error3 = dlerror();

  void (*mdp_Diffu_module)(double*, double*, DiffuParams) = (void (*)(double*, double*, DiffuParams)) dlsym(handle, "wrapper_Diffu_module");
  const char *dlsym_error4 = dlerror();

  void (*mdp_data_copy)(double*, double*, int, int) = (void (*)(double*, double*, int, int)) dlsym(handle, "wrapper_data_copy");
  const char *dlsym_error5 = dlerror();

  if ( (dlsym_error1 != NULL) || (dlsym_error2 != NULL) || (dlsym_error3 != NULL) || (dlsym_error4 != NULL) || (dlsym_error5 != NULL))
  {
    if (dlsym_error1 != NULL) cerr << "Cannot load symbol " << "wrapper_calc_x" << ": " << dlsym_error1 << '\n';
    if (dlsym_error2 != NULL) cerr << "Cannot load symbol " << "wrapper_data_set_value" << ": " << dlsym_error2 << '\n';
    if (dlsym_error3 != NULL) cerr << "Cannot load symbol " << "wrapper_output_data_console" << ": " << dlsym_error3 << '\n';
    if (dlsym_error4 != NULL) cerr << "Cannot load symbol " << "wrapper_Diffu_module" << ": " << dlsym_error4 << '\n';
    if (dlsym_error5 != NULL) cerr << "Cannot load symbol " << "wrapper_data_copy" << ": " << dlsym_error5 << '\n';
    dlclose(handle);
  }else{
    // Parameter initialization
    DiffuParams mdp; // declared in diffu_eqn_ser.h
    mdp.time_start = _time_old;
    mdp.time_end = _time;
    mdp.x0 = 0.0;
    mdp.dx = 0.5;
    mdp.nghost = 1;
    mdp.iLower = 1;
    mdp.iUpper = 11;
    mdp.numdata = 2 * mdp.nghost + mdp.iUpper - mdp.iLower + 1;
    mdp.iboundaryL = mdp.iLower - mdp.nghost;
    mdp.iboundaryU = mdp.iUpper + mdp.nghost;
    mdp.DiffCoef = 1.0;
    mdp.bc_Lval_Dirichlet = 1.0;
    mdp.bc_Uval_Dirichlet = -1.0;

    double *mdp_data;
    mdp_data = new double[mdp.numdata];
    double *mdp_x;
    mdp_x = new double[mdp.numdata];

    if (_time_old < 1e-14) //initialize the data
    {
      mdp_calc_x(_diffu_x, mdp.iLower, mdp.iUpper, mdp.nghost, mdp.dx, mdp.x0);
      mdp_data_set_value(_diffu_data_saved, mdp.iLower, mdp.iUpper, 1.0);
    }

    //copy the data to buffers
    mdp_data_copy(mdp_x, _diffu_x, mdp.iLower, mdp.iUpper);
    mdp_data_copy(mdp_data, _diffu_data_saved, mdp.iLower, mdp.iUpper);

    mdp_data_set_value(mdp_data, mdp.iLower, mdp.iUpper, 0.0);

    mdp_output_data_console(mdp_data, mdp.iLower, mdp.iUpper, mdp_x, mdp.time_start);
    mdp_Diffu_module(mdp_data, mdp_x, mdp);
    mdp_output_data_console(mdp_data, mdp.iLower, mdp.iUpper, mdp_x, mdp.time_end);

    //update the data
    mdp_data_copy(_diffu_data_saved, mdp_data, mdp.iLower, mdp.iUpper);

    dlclose(handle);
  }
  ////////////////
}

void
MyDiffuTimeStepper::postExecute()
{
}

/* Indication of whether the Monte Carlo solve converged - assume this will
   always be true. */
bool
MyDiffuTimeStepper::converged()
{
  return true;
}
