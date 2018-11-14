
#ifndef MYDIFFUEXECUTIONER_H
#define MYDIFFUEXECUTIONER_H

#include "Transient.h"

class MyDiffuExecutioner;

template<>
InputParameters validParams<MyDiffuExecutioner>();

class MyDiffuExecutioner : public Transient
{
 public:
   MyDiffuExecutioner(const InputParameters & parameters);

   virtual void init();
};
#endif //MYDIFFUEXECUTIONER_H
