
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


pair<int,int> SteppingAction::WhichXYbin(double xpos, double ypos) {
  G4double moduleSize = 121.2 * mm;
  G4double cellSize = moduleSize / 6;
  int ix = int (floor(xpos/cellSize)) + ecalGranularity/2;
  int iy = int (floor(ypos/cellSize)) + ecalGranularity/2;
  if (ix < 0 || iy < 0 || ix >= ecalGranularity || iy >= ecalGranularity) {
    return pair<int,int> (-1, -1);
  }
  return pair<int,int> (ix, iy);
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

   if (step->GetTrack()->GetTotalEnergy() > 50 * GeV) {
     //     G4cout << volume->GetName()<< " xyz1="<< pos1.x()<<'/'<<pos1.y()<<'/'<<pos1.z()<< " track energy:"<<step->GetTrack()->GetTotalEnergy() / GeV <<'/'<<edep/GeV<<" GeV, PID: " << step->GetTrack()->GetDefinition()->GetParticleName() << G4endl;
   }

  pair<int,int> ixy = WhichXYbin(pos1.x(),pos1.y());


  // G4cout << "sqr " << pos1.z() << " " << pos2.z() << " " << pos1.x() << " " << pos2.x() << " " << edep << " " << step->GetTrack()->GetDefinition()->GetParticleName() << " " << step->GetTrack()->GetKineticEnergy() << G4endl;
      
  //  G4cout << "part " << pos1.x() << " " << pos1.y() << " " << pos1.z() << " " << edep << G4endl;
  //  G4cout << "cell " << ixy.first << ':' << ixy.second << "  " << pos1.x()/(121.2 * mm / 6) + 15. << ':' << pos1.y()/(121.2 * mm / 6)+15.  << "   " << ecalGranularity << G4endl;
  // // int mybin = 0;
  // G4cout << "zbin " << WhichZBin(pos1.z()) << " " << mybin << " " << mybin%100 << std::endl;
  
  RunData* runData = static_cast<RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

   if ( volume == fDetConstruction->GetAbsorberPV() ) {
    runData->AddAbsDe (edep);
  }
  else if ( volume == fDetConstruction->GetGapPV() ) {
    if (ixy.first >= 0) {
      runData->AddSciDe(ixy.first, ixy.second, edep);
      //      cout << "adding "<<edep/MeV<<" MeV to cell x:y: "<<ixy.first<<':'<<ixy.second<<endl;
    }
  }  
}

