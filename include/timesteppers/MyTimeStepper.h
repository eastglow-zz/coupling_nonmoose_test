#ifndef MYTIMESTEPPER_H
#define MYTIMESTEPPER_H

#include "TimeStepper.h"

class MyTimeStepper;

template<>
InputParameters validParams<MyTimeStepper>();

class MyTimeStepper : public TimeStepper
{
public:
  MyTimeStepper(const InputParameters & parameters);

protected:
  virtual Real computeInitialDT() override;
  virtual Real computeDT() override;
  virtual void step() override;
  virtual void postExecute() override;
  virtual bool converged() override;

private:
  Real _dt;
  std::string _ext_lib_path_name;
  std::string _func_name;
};

#endif //MYTIMESTEPPER_H
