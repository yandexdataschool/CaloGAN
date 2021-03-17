// F. Ratnikov

#include "StackingAction.hh"

#include "RunData.hh"
#include "CaloConfiguration.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"

using namespace std;

StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack) {
  if (fCaloConfiguration->cherenkovPhotonsSignal ()) {
    if(aTrack->GetDefinition () == G4OpticalPhoton::OpticalPhotonDefinition()
       && aTrack->GetVolume () == fCaloConfiguration->sensitivePV()) {
      // if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
      const G4ThreeVector& pos = aTrack->GetPosition ();
      // G4double time = aTrack->GetGlobalTime ();
      // const G4ThreeVector& direction = aTrack->GetMomentumDirection ()
      G4double energy = aTrack->GetTotalEnergy();
      
      // here one can incert photon transport probability and timing 
      
      RunData* runData = static_cast<RunData*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
      runData->AddSignalEnergy(*fCaloConfiguration, pos, energy/MeV); // store energy
      return fKill;
    }
  }
  return fUrgent;
}
