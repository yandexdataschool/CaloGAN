
#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>
#include <map>

const int ecalGranularity = 5*6;

class RunData : public G4Run {
public:
  RunData();
  virtual ~RunData();

  void AddSciDe(G4int ix, G4int iy, G4double de);
  void AddAbsDe(G4double de);
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

inline void RunData::AddSciDe(G4int ix, G4int iy, G4double de) {
  fEnergyDeposit[ix + iy * ecalGranularity] += de;
  fTotalEnergySci += de;
}

inline void RunData::AddAbsDe(G4double de) {
  fTotalEnergyAbsorb += de;
}

#endif

