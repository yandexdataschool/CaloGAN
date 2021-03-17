
#include "RunData.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
//#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

using namespace std;

RunData::RunData() :
  G4Run(),
  fEnergyDeposit (),
  fParticlePoint (3),
  fParticleMomentum (3)
{
  Reset ();
  // Set Ntuple
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  int ecalNT = man->CreateNtuple ("ecalNT", "ECAL Values");
  ntupleId ["EnergyDeposit"] = pair<int, int> (ecalNT, man->CreateNtupleDColumn (ecalNT, "EnergyDeposit", fEnergyDeposit));
  ntupleId ["TotalEnergySci"] = pair<int, int> (ecalNT, man->CreateNtupleDColumn (ecalNT, "TotalEnergySci"));
  ntupleId ["TotalEnergyAbsorb"] = pair<int, int> (ecalNT, man->CreateNtupleDColumn (ecalNT, "TotalEnergyAbsorb"));
  ntupleId ["ParticlePoint"] = pair<int, int> (ecalNT, man->CreateNtupleDColumn (ecalNT, "ParticlePoint", fParticlePoint));
  ntupleId ["ParticleMomentum"] = pair<int, int> (ecalNT, man->CreateNtupleDColumn (ecalNT, "ParticleMomentum", fParticleMomentum));
  ntupleId ["ParticlePDG"] = pair<int, int> (ecalNT, man->CreateNtupleIColumn (ecalNT, "ParticlePDG"));
  man->FinishNtuple(ecalNT);
}

RunData::~RunData() {}



void RunData::SetParticle (G4int pdg, const G4ThreeVector& point, const G4ThreeVector& momentum) {
  fParticlePDG = pdg;
  for (size_t i = 0; i < 3; ++i) {
    fParticlePoint[i] = point[i] / cm;
    fParticleMomentum[i] = momentum[i] / GeV;
  }
}

void RunData::FillPerEvent() {
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->FillNtupleDColumn (ntupleId ["TotalEnergySci"].first, ntupleId ["TotalEnergySci"].second, fTotalEnergySci);
  man->FillNtupleDColumn (ntupleId ["TotalEnergyAbsorb"].first, ntupleId ["TotalEnergyAbsorb"].second, fTotalEnergyAbsorb);
  man->FillNtupleIColumn (ntupleId ["ParticlePDG"].first, ntupleId ["ParticlePDG"].second, fParticlePDG);
  man->AddNtupleRow(ntupleId ["ParticlePDG"].first);
  Reset();
}

void RunData::Reset() {
  fill (fEnergyDeposit.begin(), fEnergyDeposit.end(), 0.);
  fTotalEnergySci = fTotalEnergyAbsorb = 0.;
  fill (fParticlePoint.begin(), fParticlePoint.end(), 0);
  fill (fParticleMomentum.begin(), fParticleMomentum.end(), 0);
  fParticlePDG = 0;
}

void RunData::AddSignalEnergy(const CaloConfiguration& config, const G4ThreeVector pos, G4double de) {
  double cellSize = config.cellSize();
  int calGranularity = config.calGranularity();
  int nLogLayers = 1;
 
  int ix = int (floor(pos.x()/cellSize)) + calGranularity/2;
  int iy = int (floor(pos.y()/cellSize)) + calGranularity/2;
  int iz = 0;
  if (config.frontCellLength () > 0) { // split two layers
    double zSplitWorld = config.frontCellLength () - config.calorLength () / 2;
    if (pos.z() > zSplitWorld) iz = 1;
    nLogLayers = 2;
  }
  else if (config.isShashlik() && (config.shashlik().aggregateLayers < config.shashlik().nLayers)) {
    double layerThickness = config.shashlik().sciDepth + config.shashlik().absorberDepth;
    int nLayers = config.shashlik().nLayers;
    iz = int (floor(pos.z()/layerThickness)) + nLayers/2;
    iz = int (iz / config.shashlik().aggregateLayers);
    nLogLayers = nLayers / config.shashlik().aggregateLayers;
  }
  if (fEnergyDeposit.empty()) { //initiate container
    fEnergyDeposit = std::vector<G4double> (calGranularity*calGranularity*nLogLayers);
  }
  int index = (ix * calGranularity + iy) * nLogLayers + iz;
  if (index >= 0 && index < int(fEnergyDeposit.size())) {
    fEnergyDeposit[ (ix * calGranularity + iy) * nLogLayers + iz] += de;
  }
  fTotalEnergySci += de;
}

void RunData::AddAbsorberEnergy(G4double de) {
  fTotalEnergyAbsorb += de;
}

