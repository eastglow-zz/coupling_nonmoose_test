//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef COUPLING_NONMOOSE_TESTAPP_H
#define COUPLING_NONMOOSE_TESTAPP_H

#include "MooseApp.h"

class coupling_nonmoose_testApp;

template <>
InputParameters validParams<coupling_nonmoose_testApp>();

class coupling_nonmoose_testApp : public MooseApp
{
public:
  coupling_nonmoose_testApp(InputParameters parameters);
  virtual ~coupling_nonmoose_testApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void registerObjectDepends(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void associateSyntaxDepends(Syntax & syntax, ActionFactory & action_factory);
  static void registerExecFlags(Factory & factory);
};

#endif /* COUPLING_NONMOOSE_TESTAPP_H */
