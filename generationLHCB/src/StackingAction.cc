// F. Ratnikov

#include "StackingAction.hh"

#include "RunData.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"

using namespace std;

StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack) {
  if(aTrack->GetDefinition () == G4OpticalPhoton::OpticalPhotonDefinition()
     && aTrack->GetVolume () == fDetConstruction->GetSensitivePV()) {
    // if(aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")
    const G4ThreeVector& pos = aTrack->GetPosition ();
    // G4double time = aTrack->GetGlobalTime ();
    // const G4ThreeVector& direction = aTrack->GetMomentumDirection ()
    G4double energy = aTrack->GetTotalEnergy();
    
    // here one can incert photon transport probability and timing 
    
    // Save energy and kill photon
    int ix = int (floor(pos.x()/cellSize)) + calGranularityX/2;
    int iy = int (floor(pos.y()/cellSize)) + calGranularityY/2;
    int iz = int (floor(pos.z()/layerThickness)) + nLayers/2;
    iz = int (iz / aggregateLayers);
		
    if (ix >= 0 && ix < calGranularityX &&
	iy >= 0 && iy < calGranularityY &&
	iz >= 0 && iz < nLogLayers) {
      RunData* runData = static_cast<RunData*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
      runData->AddSciDe(ix, iy, iz, energy/MeV); // store energy
      // cout << "StackingAction adding: "<<ix<<':'<<iy<<':'<<iz<<" energy: "<<energy<< " XYZ: " << pos.x() << ':'<< pos.y() << ':'<< pos.z() << endl;
    }
    return fKill;
  }
  return fUrgent;
}
