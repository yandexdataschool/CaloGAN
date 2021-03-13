// Fedor Ratnikov replica from
// https://gitlab.cern.ch/spacal-rd/spacal-simulation/-/blob/master/src/G4EmUserPhysics.cc

#include "OpticalPhotonPhysics.hh"
#include "G4ParticleDefinition.hh"
#include "G4LossTableManager.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VProcess.hh"
#include "G4EmProcessOptions.hh"
#include "G4AntiProton.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalPhotonPhysics::OpticalPhotonPhysics(const G4int& scint, const G4int& cher) :
  G4VPhysicsConstructor("User Optical Options"),
  switchOnScintillation(scint),
  switchOnCerenkov(cher)
{
  G4LossTableManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalPhotonPhysics::~OpticalPhotonPhysics()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalPhotonPhysics::ConstructParticle()
{
  //G4AntiProton::AntiProton();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalPhotonPhysics::ConstructProcess()
{
  theCerenkovProcess = new G4Cerenkov("Cerenkov");
  theScintillationProcess = new G4Scintillation("Scintillation");
  theAbsorptionProcess = new G4OpAbsorption();
  theRayleighScatteringProcess = new G4OpRayleigh();
  theMieHGScatteringProcess = new G4OpMieHG();
  theBoundaryProcess = new G4OpBoundaryProcess();

  //theCerenkovProcess->DumpPhysicsTable();
  //theScintillationProcess->DumpPhysicsTable();
  //theRayleighScatteringProcess->DumpPhysicsTable();

  theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
  theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  theCerenkovProcess->SetTrackSecondariesFirst(true);

  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);

  theScintillationProcess->SetFiniteRiseTime(true);

  // Use Birks Correction in the Scintillation process
  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  theScintillationProcess->AddSaturation(emSaturation);

  G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable() -> GetIterator();
  theParticleIterator -> reset();
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if( (switchOnScintillation) && (theScintillationProcess->IsApplicable(*particle)) )
    {
      pmanager->AddProcess(theScintillationProcess);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    }

    if( (switchOnCerenkov) && (theCerenkovProcess->IsApplicable(*particle)) )
    {
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
    }

    if (particleName == "opticalphoton")
    {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theMieHGScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
    }
  }
}

// void OpticalPhotonPhysics::SetCuts()
// {
//   G4double cut = 0.1*CLHEP::mm;
//   SetDefaultCutValue(cut);
// }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
