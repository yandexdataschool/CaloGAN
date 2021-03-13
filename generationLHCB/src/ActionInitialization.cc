
#include "ActionInitialization.hh"
//#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4MTRunManager.hh"
#include "DetectorConstruction.hh"


ActionInitialization::ActionInitialization (const DetectorConstruction* detector)
  : G4VUserActionInitialization(),
    fDetector (detector)
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
  SetUserAction(new SteppingAction(fDetector));
}  

