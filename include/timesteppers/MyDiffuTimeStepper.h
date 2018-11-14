#ifndef MYDIFFUTIMESTEPPER_H
#define MYDIFFUTIMESTEPPER_H

#include "TimeStepper.h"

class MyDiffuTimeStepper;

template<>
InputParameters validParams<MyDiffuTimeStepper>();

class MyDiffuTimeStepper : public TimeStepper
{
public:
  MyDiffuTimeStepper(const InputParameters & parameters);

protected:
  virtual Real computeInitialDT() override;
  virtual Real computeDT() override;
  virtual void step() override;
  virtual void postExecute() override;
  virtual bool converged() override;

private:
  Real _dt;
  std::string _ext_lib_path_name;
  double _diffu_data_saved[23];
  double _diffu_x[23];
};

#endif //MYDIFFUTIMESTEPPER_H
