
#include "SteppingAction.hh"
#include "RunData.hh"
#include "CaloConfiguration.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

class G4VPhysicalVolume;

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
                      const CaloConfiguration* config)
  : G4UserSteppingAction(),
    fConfiguration(config)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ 
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // only interested in dE for shashlik
  if (!fConfiguration->isShashlik()) return;
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep <= 0) return;
  
// Collect energy and track length step by step

  // get volume of the current step
   G4VPhysicalVolume* volume 
     = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  // G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
  //  cout << "particle : " << step->GetTrack()->GetDefinition()->GetParticleName() 
  // 	<< " volume: " << volume->GetName () << '('<< volume->GetLogicalVolume ()->GetName ()
  // 	<< " energy: " << step->GetTrack()->GetTotalEnergy()
  // 	<< " position: " << pos.x()<<'/'<<pos.y()<<'/'<<pos.z()
  // 	<< endl;


  RunData* runData = static_cast<RunData*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
   
  // energy deposit
  if (volume == fConfiguration->absorberPV() ) {
    runData->AddAbsorberEnergy (edep/MeV);
  }
  else if (volume == fConfiguration->sensitivePV() ) {
    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
    runData->AddSignalEnergy (*fConfiguration, pos, edep/MeV);
  }
}

