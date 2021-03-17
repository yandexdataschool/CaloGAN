
#include "SteppingAction.hh"
#include "RunData.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
                      const DetectorConstruction* detectorConstruction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction)
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
  if (edep <= 0) return 
  
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
  else if (volume == config->sensitivePV() ) {
    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
    runData->AddSignalEnergy (config, pos, edep/MeV);
  }
}


    
    if (volume->GetName () == "Scintillator") {  // Shashlik option
      if (edep <= 0) return;
    }
    // else {  // SPACAL option
    //   G4Track* theTrack = step->GetTrack () ;
    //   G4ParticleDefinition* particleType = theTrack->GetDefinition ();
    //   if( particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {	
    // 	edep = theTrack->GetTotalEnergy();
    // 	theTrack->SetTrackStatus(fStopAndKill); // stop this photon
    //   }
    // }
      
    G4ThreeVector pos1 = step->GetPreStepPoint()->GetPosition();
    int ix = int (floor(pos1.x()/cellSize)) + calGranularityX/2;
    int iy = int (floor(pos1.y()/cellSize)) + calGranularityY/2;
    int iz = int (floor(pos1.z()/layerThickness)) + nLayers/2;
    iz = int (iz / aggregateLayers);
		
    if (ix >= 0 && ix < calGranularityX &&
	iy >= 0 && iy < calGranularityY &&
	iz >= 0 && iz < nLogLayers) {
      RunData* runData = static_cast<RunData*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
       runData->AddSciDe(ix, iy, iz, edep/MeV);
      //      cout << "enter energy: "<<ix<<':'<<iy<<':'<<iz<<" energy: "<<edep<< " XYZ: " << pos1.x() << ':'<< pos1.y() << ':'<< pos1.z() << endl;
    }
  }  
}

