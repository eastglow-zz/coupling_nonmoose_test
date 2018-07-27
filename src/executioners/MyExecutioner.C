
#include "MyExecutioner.h"
//#include "MyExFncInterface.h"

template<>
InputParameters validParams<MyExecutioner>()
{
  InputParameters params = validParams<Transient>();
  return params;
}

MyExecutioner::MyExecutioner(const InputParameters & parameters) :
    Transient(parameters)
{
  //for (unsigned int i = 0; i < 10; i++)
  //{
  //  printf("%d\n",i);
  //}
}

// This method is called only one time at the start of the entire simulation.
void MyExecutioner::init() {
  Transient::init();
  //print_one_to_ten::print_one_to_ten();
}
