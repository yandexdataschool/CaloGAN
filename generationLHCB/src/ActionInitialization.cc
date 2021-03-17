
#include "ActionInitialization.hh"
//#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "G4MTRunManager.hh"
#include "CaloConfiguration.hh"

ActionInitialization::ActionInitialization (const CaloConfiguration& configuration)
  : G4VUserActionInitialization(),
    fConfiguration (&configuration)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  //  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);
  SetUserAction(new StackingAction(fConfiguration));
  SetUserAction(new SteppingAction(fConfiguration));
}  

