#include "coupling_nonmoose_testApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

// My custom Executioners
#include "MyExecutioner.h"
#include "MyDiffuExecutioner.h"

// My custom TimeSteppers
#include "MyTimeStepper.h"
#include "MyDiffuTimeStepper.h"

template <>
InputParameters
validParams<coupling_nonmoose_testApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

coupling_nonmoose_testApp::coupling_nonmoose_testApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  coupling_nonmoose_testApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  coupling_nonmoose_testApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  coupling_nonmoose_testApp::registerExecFlags(_factory);
}

coupling_nonmoose_testApp::~coupling_nonmoose_testApp() {}

void
coupling_nonmoose_testApp::registerApps()
{
  registerApp(coupling_nonmoose_testApp);
}

void
coupling_nonmoose_testApp::registerObjects(Factory & factory)
{
    Registry::registerObjectsTo(factory, {"coupling_nonmoose_testApp"});

    //Registering My custom executioners
    registerExecutioner(MyExecutioner);
    registerExecutioner(MyDiffuExecutioner);

    //Registering My custom timesteppers
    registerTimeStepper(MyTimeStepper);
    registerTimeStepper(MyDiffuTimeStepper);
}

void
coupling_nonmoose_testApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"coupling_nonmoose_testApp"});

  /* Uncomment Syntax parameter and register your new production objects here! */
}

void
coupling_nonmoose_testApp::registerObjectDepends(Factory & /*factory*/)
{
}

void
coupling_nonmoose_testApp::associateSyntaxDepends(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

void
coupling_nonmoose_testApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execution flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
coupling_nonmoose_testApp__registerApps()
{
  coupling_nonmoose_testApp::registerApps();
}

extern "C" void
coupling_nonmoose_testApp__registerObjects(Factory & factory)
{
  coupling_nonmoose_testApp::registerObjects(factory);
}

extern "C" void
coupling_nonmoose_testApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  coupling_nonmoose_testApp::associateSyntax(syntax, action_factory);
}

extern "C" void
coupling_nonmoose_testApp__registerExecFlags(Factory & factory)
{
  coupling_nonmoose_testApp::registerExecFlags(factory);
}
