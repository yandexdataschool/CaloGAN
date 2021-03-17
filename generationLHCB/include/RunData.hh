
#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>
#include <map>

#include "CaloConfiguration.hh"

class RunData : public G4Run {
public:
  RunData();
  virtual ~RunData();

  void AddSignalEnergy(const CaloConfiguration& config, const G4ThreeVector pos, G4double de);
  void AddAbsorberEnergy(G4double de);
  void SetParticle (G4int pdg, const G4ThreeVector& point, const G4ThreeVector& momentum);
  void FillPerEvent();
  void Reset();

private:
  std::vector<G4double>  fEnergyDeposit;
  G4double fTotalEnergySci;
  G4double fTotalEnergyAbsorb;
  std::vector<G4double> fParticlePoint;
  std::vector<G4double> fParticleMomentum;
  G4int fParticlePDG;
  std::map< std::string, std::pair<int, int> > ntupleId;
};

#endif

