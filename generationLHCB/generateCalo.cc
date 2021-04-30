// Fedor Ratnikov

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "OpticalPhotonPhysics.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

#include "time.h"
#include <unistd.h>

#include "CaloConfiguration.hh"

using namespace std;


int main(int argc,char** argv)
{
  G4long seed = abs(((time(NULL) * 181) * ((getpid() - 83) * 359)) % 104729);

  // parameters
  int partPDG = 22; // gamma
  double spotCenterX = 0;
  double spotCenterY = 0;
  double spotSize = -1.01*CLHEP::cm;
  double spotAngleX = 0.;
  double spotAngleY = 0.;
  double spotAngleSize = 0.175;  // 10 degree
  double eMin = 1.*CLHEP::GeV;
  double eMax = 100.*CLHEP::GeV;
  int eDependency = -1;
  int nEvents = 100;
  int iCase = 0;
  string fileName = string("caloGAN_")+string(G4UIcommand::ConvertToString (G4int(seed)));
  int nBatch = 1;
  int nLog = 1000;
  double zOffset = 15*CLHEP::cm;
  CaloConfiguration config;
  

  int iArg = 1;
  while (iArg < argc) {
    G4String cmd (argv[iArg++]);
    if (cmd == "-pdg") {
      partPDG = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-seed") {
      seed = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-xy") {
      spotCenterX = G4UIcommand::ConvertToDouble(argv[iArg++])*CLHEP::cm;
      spotCenterY = G4UIcommand::ConvertToDouble(argv[iArg++])*CLHEP::cm;
    }
    else if (cmd == "-size") {
      spotSize = G4UIcommand::ConvertToDouble(argv[iArg++])*CLHEP::cm;
    }
    else if (cmd == "-dxydz") {
      spotAngleX = G4UIcommand::ConvertToDouble(argv[iArg++]);
      spotAngleY = G4UIcommand::ConvertToDouble(argv[iArg++]);
    }
    else if (cmd == "-ddxydz") {
      spotAngleSize = G4UIcommand::ConvertToDouble(argv[iArg++]);
    }
    else if (cmd == "-eminmax") {
      eMin = G4UIcommand::ConvertToDouble(argv[iArg++])*CLHEP::GeV;
      eMax = G4UIcommand::ConvertToDouble(argv[iArg++])*CLHEP::GeV;
    }
    else if (cmd == "-edep") {
      eDependency = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-case") {
      iCase = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-nev") {
      nEvents = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-fName") {
      fileName = string(argv[iArg++]);
    }
    else if (cmd == "-batch") {
      nBatch = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-nlog") {
      nLog = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-zoffset") {
      zOffset = G4UIcommand::ConvertToDouble(argv[iArg++]);
    }
 }

  if (iCase) {
    fileName =  string("caloGAN_case")+string(G4UIcommand::ConvertToString (G4int(iCase))) + string ("_")+string(G4UIcommand::ConvertToString (G4int(seed)));
  }
  if (iCase == 11) {
    // shashlik nonsplit
    ShashlikCaloConfiguration shashlik;
    shashlik.modules = 5;
    shashlik.cells = 6;
    shashlik.sciDepth = 4*mm;
    shashlik.absorberDepth = 2*mm;
    shashlik.nLayers = 66;
    shashlik.aggregateLayers = 66;
    shashlik.frontCellLength = 0;
    config = CaloConfiguration (shashlik);
  }
  else if (iCase == 12) {
    // shashlik split
    ShashlikCaloConfiguration shashlik;
    shashlik.modules = 5;
    shashlik.cells = 6;
    shashlik.sciDepth = 4*mm;
    shashlik.absorberDepth = 2*mm;
    shashlik.nLayers = 66;
    shashlik.aggregateLayers = 66;
    shashlik.frontCellLength = 110*mm;
    config = CaloConfiguration (shashlik);
  }
  else if (iCase == 21) {
    //spacal W/GAGG nonsplit
    SpacalCaloConfiguration spacal;
    spacal.modules = 5;
    spacal.cells = 8;
    spacal.fibres = 72;
    spacal.fibreSize = 1*mm;
    spacal.airGap = 0.1*mm;
    spacal.length = 150*mm;
    spacal.frontCellLength = 0;
    spacal.fibreMaterial = SpacalCaloConfiguration::GAGG;
    spacal.absorberMaterial = SpacalCaloConfiguration::TUNGSTEN;
    config = CaloConfiguration (spacal);
  }
  else if (iCase == 22) {
    //spacal W/GAGG nonsplit
    SpacalCaloConfiguration spacal;
    spacal.modules = 5;
    spacal.cells = 8;
    spacal.fibres = 72;
    spacal.fibreSize = 1*mm;
    spacal.airGap = 0.1*mm;
    spacal.length = 150*mm;
    spacal.frontCellLength = 45*mm;
    spacal.fibreMaterial = SpacalCaloConfiguration::GAGG;
    spacal.absorberMaterial = SpacalCaloConfiguration::TUNGSTEN;
    config = CaloConfiguration (spacal);
  }
  else if (iCase == 31) {
    //spacal Pb/PS nonsplit
    SpacalCaloConfiguration spacal;
    spacal.modules = 5;
    spacal.cells = 4;
    spacal.fibres = 72;
    spacal.fibreSize = 1*mm;
    spacal.airGap = 0.1*mm;
    spacal.length = 290*mm;
    spacal.frontCellLength = 0;
    spacal.fibreMaterial = SpacalCaloConfiguration::POLYSTYRENE;
    spacal.absorberMaterial = SpacalCaloConfiguration::LEAD;
    config = CaloConfiguration (spacal);
  }
  else if (iCase == 32) {
    //spacal Pb/PS split
    SpacalCaloConfiguration spacal;
    spacal.modules = 5;
    spacal.cells = 4;
    spacal.fibres = 72;
    spacal.fibreSize = 1*mm;
    spacal.airGap = 0.1*mm;
    spacal.length = 290*mm;
    spacal.frontCellLength = 80*mm;
    spacal.fibreMaterial = SpacalCaloConfiguration::POLYSTYRENE;
    spacal.absorberMaterial = SpacalCaloConfiguration::LEAD;
    config = CaloConfiguration (spacal);
  }

  setenv("GAN_FNAME",fileName.c_str(),1);  // let RunAction to set filename properly

  
  // Choose the Random engine
  //
  // G4Random::setTheEngine(new CLHEP::RanecuEngine);

  CLHEP::HepRandom::setTheSeed(seed);
  
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;


  // Set mandatory initialization classes
  //
  DetectorConstruction* detConstruction = new DetectorConstruction(config);
  runManager->SetUserInitialization(detConstruction);

  // Physics
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  if (config.cherenkovPhotonsSignal ()) { // add Cerenkov's photons production
    const bool switchCherenkov = true;
    const bool switchScintillation = false; // never want it 
    physicsList->RegisterPhysics (new OpticalPhotonPhysics(switchScintillation, switchCherenkov)); // add cherenkov and scintillation
  }
  runManager->SetUserInitialization(physicsList);
    
  ActionInitialization* actionInitialization
    = new ActionInitialization(config);
  runManager->SetUserInitialization(actionInitialization);

  runManager->Initialize();   
  runManager->ConfirmBeamOnCondition();
  runManager->ConstructScoringWorlds();
  runManager->RunInitialization(); // this is part of BeamOn and needs be done (at least) to set GeomClosed status
   
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
   
  //int partPDG = 11; // electron
  G4ParticleDefinition* g4pd = ptable->FindParticle( partPDG );

  cout << "Detector parameters: " << endl
       << " detector dimensions: " << config.calorSize()/mm << 'x' << config.calorSize()/mm << 'x' << config.calorLength()/mm << "mm^3" << endl
       << " granularity: " << config.calGranularity() << 'x' << config.calGranularity()  << endl
       << " cell size: " << config.cellSize()/mm << 'x' << config.cellSize()/mm << "mm^2"
       << endl;

  cout << "Job parameters:" << endl
       << " spot position X Y size: " << spotCenterX << ' ' << spotCenterY << ' ' << spotSize << endl
       << " angle direction and spread: " << spotAngleX << ' ' << spotAngleY << ' ' << spotAngleSize << endl
       << " energy min max: " << eMin << ' ' << eMax << endl
       << " events: " << nEvents << " by batches of " << nBatch << " events" << endl
       << "   file: " << fileName << ".root"
       << endl;
  
  int iBatch = nBatch;
  
  double E = 0;
  double pz = 0;
  double dxdz = 0;
  double dydz = 0;
  double x0 = 0;
  double y0 = 0;


  for ( int iev=0; iev<nEvents; ++iev )
    {
      
      
      // NOTE: there maybe several ctor's for prim.vtx
      //       I think you can also construct from a a CLHP vector, etc.
      //       you may also specify units if values are non-zero
      //       ( time is typically in ns)
      //


      if (++iBatch >= nBatch) { // generate new kinematics
	E = 0;
	while (1) {
	  E = G4RandFlat::shoot(eMin, eMax);
	  if (eDependency == 0) break;
	  double w = 0;
	  if (eDependency < 0) { 
	    w = pow (eMin/E, -eDependency); // 1/E^n dependency
	  }
	  if (eDependency > 0) {
	    w = pow (E/eMax, eDependency); // E^n dependency
	  }
	  if (G4RandFlat::shoot(0., 1.) < w) break;
	}
	
	dxdz = G4RandGauss::shoot (spotAngleX, spotAngleSize);
	dydz = G4RandGauss::shoot (spotAngleY, spotAngleSize);
	pz = E / sqrt (1. + dxdz*dxdz + dydz*dydz);
	
	x0 = G4RandFlat::shoot(spotCenterX-spotSize, spotCenterX+spotSize); 
	y0 = G4RandFlat::shoot(spotCenterY-spotSize, spotCenterY+spotSize); 
	x0 += -dxdz*zOffset;
	y0 += -dydz*zOffset;
	iBatch = 0;
      }
	
	G4PrimaryParticle* g4part = new G4PrimaryParticle( g4pd, pz*dxdz, pz*dydz, pz, E );
      
	G4PrimaryVertex* g4vtx = new G4PrimaryVertex( x0, y0, -0.5*config.calorLength (), 0. ); // xyzt
      g4vtx->SetPrimary(g4part);
      G4Event* g4evt = new G4Event( iev );
      g4evt->AddPrimaryVertex(g4vtx);

      if (!(iev%nLog)) cout<<iev<<" generated parameters: "<<" E="<<E<<" x:y="<<x0<<':'<<y0<<" dx:y="<<dxdz<<':'<<dydz<<endl;
      
      // NOTE: you can generate as mant vertices as you wish, 
      //       with many particles attached to each vertex
      
      // now just process the event through your detector
      //
      //
      G4EventManager::GetEventManager()->ProcessOneEvent( g4evt );
      
      // NOTE: cant't remember what this one if for...
      //       I think this is somehow related to collecting stuff from Sensitive Detector's
      //       all in all, I found it in my code and have decided that "it wont hurt"
      //
      runManager->AnalyzeEvent( g4evt );

      delete g4evt;
      g4evt = 0;
   
   }
  runManager->RunTermination();

  delete runManager;
}

