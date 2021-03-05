
#include "SteppingAction.hh"
#include "RunData.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

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
// Collect energy and track length step by step

  // get volume of the current step
   G4VPhysicalVolume* volume 
     = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep <= 0) return;
  
  G4StepPoint* point1 = step->GetPreStepPoint();
  G4ThreeVector pos1 = point1->GetPosition();

   // if (step->GetTrack()->GetTotalEnergy() > 50 * GeV) {
   //   G4cout << volume->GetName()<< " xyz1="<< pos1.x()<<'/'<<pos1.y()<<'/'<<pos1.z(<< " track energy:"<<step->GetTrack()->GetTotalEnergy()<<'/'<<edep<<" PID" << step->GetTrack()->GetDefinition()->GetParticleName() << G4endl;
   // }


  // G4cout << "sqr " << pos1.z() << " " << pos2.z() << " " << pos1.x() << " " << pos2.x() << " " << edep << " " << step->GetTrack()->GetDefinition()->GetParticleName() << " " << step->GetTrack()->GetKineticEnergy() << G4endl;
      
  // G4cout << "sqr " << pos1.x() << " " << pos1.y() << " " << pos1.z() << " " << edep << G4endl;
  // // int mybin = 0;
  // G4cout << "zbin " << WhichZBin(pos1.z()) << " " << mybin << " " << mybin%100 << std::endl;
  
  RunData* runData = static_cast<RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

   if ( volume == fDetConstruction->GetAbsorberPV() ) {
    runData->AddAbsDe (edep);
  }
  else if ( volume == fDetConstruction->GetScintillatorPV() ) {

    int ix = int (floor(pos1.x()/cellSize)) + calGranularityX/2;
    int iy = int (floor(pos1.y()/cellSize)) + calGranularityY/2;
    int iz = int (floor(pos1.z()/layerThickness)) + nLayers/2;
    iz = int (iz / aggregateLayers);
		
    if (ix >= 0 && ix < calGranularityX &&
	iy >= 0 && iy < calGranularityY &&
	iz >= 0 && iz < nLogLayers) {
      runData->AddSciDe(ix, iy, iz, edep);
      //      cout << "enter energy: "<<ix<<':'<<iy<<':'<<iz<<" energy: "<<edep<< " XYZ: " << pos1.x() << ':'<< pos1.y() << ':'<< pos1.z() << endl;
    }
  }  
}

