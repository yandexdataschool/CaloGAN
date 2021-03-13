// Fedor Ratnikov

#ifndef OpticalPhotonPhysics_h
#define OpticalPhotonPhysics_h 1

#include "G4VPhysicsConstructor.hh"
//#include "globals.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;


class OpticalPhotonPhysics : public G4VPhysicsConstructor
{
public:

  OpticalPhotonPhysics(const G4int& scint, const G4int& cher);

  virtual ~OpticalPhotonPhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  // virtual void SetCuts();

private:
  G4int switchOnScintillation;
  G4int switchOnCerenkov;

  G4Cerenkov * theCerenkovProcess;
  G4Scintillation * theScintillationProcess;
  G4OpAbsorption * theAbsorptionProcess;
  G4OpRayleigh * theRayleighScatteringProcess;
  G4OpMieHG * theMieHGScatteringProcess;
  G4OpBoundaryProcess * theBoundaryProcess;

};


#endif
