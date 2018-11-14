
#include "MyDiffuExecutioner.h"

template<>
InputParameters validParams<MyDiffuExecutioner>()
{
  InputParameters params = validParams<Transient>();
  return params;
}

MyDiffuExecutioner::MyDiffuExecutioner(const InputParameters & parameters) :
    Transient(parameters)
{
}

// This method is called only one time at the start of the entire simulation.
void MyDiffuExecutioner::init() {
  Transient::init();
}
