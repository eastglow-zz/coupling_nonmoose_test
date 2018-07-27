
#ifndef MYEXECUTIONER_H
#define MYEXECUTIONER_H

#include "Transient.h"

class MyExecutioner;

template<>
InputParameters validParams<MyExecutioner>();

class MyExecutioner : public Transient
{
 public:
   MyExecutioner(const InputParameters & parameters);

   virtual void init();
};
#endif //MYEXECUTIONER_H
