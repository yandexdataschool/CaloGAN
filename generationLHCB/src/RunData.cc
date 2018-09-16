
#include "RunData.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
//#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

using namespace std;

RunData::RunData() :
  G4Run(),
  fEnergyDeposit (calGranularityX*calGranularityY*nLogLayers),
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

