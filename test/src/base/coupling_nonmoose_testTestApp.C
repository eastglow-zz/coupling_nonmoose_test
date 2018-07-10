//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "coupling_nonmoose_testTestApp.h"
#include "coupling_nonmoose_testApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<coupling_nonmoose_testTestApp>()
{
  InputParameters params = validParams<coupling_nonmoose_testApp>();
  return params;
}

coupling_nonmoose_testTestApp::coupling_nonmoose_testTestApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  coupling_nonmoose_testApp::registerObjectDepends(_factory);
  coupling_nonmoose_testApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  coupling_nonmoose_testApp::associateSyntaxDepends(_syntax, _action_factory);
  coupling_nonmoose_testApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  coupling_nonmoose_testApp::registerExecFlags(_factory);

  bool use_test_objs = getParam<bool>("allow_test_objects");
  if (use_test_objs)
  {
    coupling_nonmoose_testTestApp::registerObjects(_factory);
    coupling_nonmoose_testTestApp::associateSyntax(_syntax, _action_factory);
    coupling_nonmoose_testTestApp::registerExecFlags(_factory);
  }
}

coupling_nonmoose_testTestApp::~coupling_nonmoose_testTestApp() {}

void
coupling_nonmoose_testTestApp::registerApps()
{
  registerApp(coupling_nonmoose_testApp);
  registerApp(coupling_nonmoose_testTestApp);
}

void
coupling_nonmoose_testTestApp::registerObjects(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new test objects here! */
}

void
coupling_nonmoose_testTestApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  /* Uncomment Syntax and ActionFactory parameters and register your new test objects here! */
}

void
coupling_nonmoose_testTestApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execute flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
coupling_nonmoose_testTestApp__registerApps()
{
  coupling_nonmoose_testTestApp::registerApps();
}

// External entry point for dynamic object registration
extern "C" void
coupling_nonmoose_testTestApp__registerObjects(Factory & factory)
{
  coupling_nonmoose_testTestApp::registerObjects(factory);
}

// External entry point for dynamic syntax association
extern "C" void
coupling_nonmoose_testTestApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  coupling_nonmoose_testTestApp::associateSyntax(syntax, action_factory);
}

// External entry point for dynamic execute flag loading
extern "C" void
coupling_nonmoose_testTestApp__registerExecFlags(Factory & factory)
{
  coupling_nonmoose_testTestApp::registerExecFlags(factory);
}
