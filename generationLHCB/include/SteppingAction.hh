
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "g4root.hh"

class DetectorConstruction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in RunData object.

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(const DetectorConstruction* detectorConstruction);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
  const DetectorConstruction* fDetConstruction;
};


#endif
