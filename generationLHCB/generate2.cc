//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: example.cc 86065 2014-11-07 08:51:15Z gcosmo $
//
/// \file example.cc
/// \brief Main program of the  example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

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

#include "time.h"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//int main(int argc,char** argv)
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
  int nEvents = 100;
  int iCase = 0;
  string fileName = string("caloGAN_")+string(G4UIcommand::ConvertToString (G4int(seed)));

  int iArg = 1;
  while (iArg < argc) {
    G4String cmd (argv[iArg++]);
    if (cmd == "-pdg") {
      partPDG = G4UIcommand::ConvertToInt(argv[iArg++]);
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
    else if (cmd == "-case") {
      iCase = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-nev") {
      nEvents = G4UIcommand::ConvertToInt(argv[iArg++]);
    }
    else if (cmd == "-fName") {
      fileName = string(argv[iArg++]);
    }
  }

  if (iCase == 1) {
    spotCenterX = 0;
    spotCenterY = 0;
    spotSize = 0;
    spotAngleX = 0.;
    spotAngleY = 0.;
    spotAngleSize = 0;
    eMin = 10.*CLHEP::GeV;
    eMax = eMin;
    fileName = string ("caloGAN_v3_case1");
  }
  else if (iCase == 2) {
    spotCenterX = -0.46881875;
    spotCenterY = -0.62373408;
    spotSize = 0;
    spotAngleX = 0.06402305307487648;
    spotAngleY = 0.06020062458846305;
    spotAngleSize = 0;
    eMin = 2.455*CLHEP::GeV;
    eMax = eMin;
    fileName = string ("caloGAN_v3_case2");
  }
  else if (iCase == 3) {
    spotCenterX = 0.76913061;
    spotCenterY = -0.59072689;
    spotSize = 0;
    spotAngleX = -0.05503867598374685;
    spotAngleY = 0.02786821220815182;
    spotAngleSize = 0;
    eMin = 29.1*CLHEP::GeV;
    eMax = eMin;
    fileName = string ("caloGAN_v3_case3");
  }
  else if (iCase == 4) {
    spotCenterX = -0.53223262;   
    spotCenterY = 0.43305124;
    spotSize = 0;
    spotAngleX = 0.10565136078827912;
    spotAngleY = -0.1443055254303747;
    spotAngleSize = 0;
    eMin = 16.9*CLHEP::GeV;
    eMax = eMin;
    fileName = string ("caloGAN_v3_case4");
  }
  else if (iCase == 5) {
    spotCenterX = 0.23922617;   
    spotCenterY = 0.93010209;
    spotSize = 0;
    spotAngleX = -0.08056594500939052;
    spotAngleY = -0.3961654441952705;
    spotAngleSize = 0;
    eMin = 34.0*CLHEP::GeV;
    eMax = eMin;
    fileName = string ("caloGAN_v3_case5");
  }

  setenv("GAN_FNAME",fileName.c_str(),1);  // let RunAction to set filename properly

  
  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  CLHEP::HepRandom::setTheSeed(seed);
  
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  DetectorConstruction* detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);
    
  ActionInitialization* actionInitialization
     = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);

  runManager->Initialize();   
  runManager->ConfirmBeamOnCondition();
  runManager->ConstructScoringWorlds();
  runManager->RunInitialization(); // this is part of BeamOn and needs be done (at least) to set GeomClosed status
   
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
   
  //int partPDG = 11; // electron
  G4ParticleDefinition* g4pd = ptable->FindParticle( partPDG );

  cout << "Job parameters:" << endl
       << "spot position X Y size: " << spotCenterX << ' ' << spotCenterY << ' ' << spotSize << endl
       << "angle direction and spread: " << spotAngleX << ' ' << spotAngleY << ' ' << spotAngleSize << endl
       << "energy min max: " << eMin << ' ' << eMax << endl
       << "events: " << nEvents << "   file: " << fileName << ".root" << endl;
  


  for ( int iev=0; iev<nEvents; ++iev )
    {
      
      
      // NOTE: there maybe several ctor's for prim.vtx
      //       I think you can also construct from a a CLHP vector, etc.
      //       you may also specify units if values are non-zero
      //       ( time is typically in ns)
      //


      double E = 0;
      while (1) {
	E = G4RandFlat::shoot(eMin, eMax);
	double w = eMin/E; // 1/E dependency
	if (G4RandFlat::shoot(0., 1.) < w) break;
      }

      double dxdz = G4RandGauss::shoot (spotAngleX, spotAngleSize);
      double dydz = G4RandGauss::shoot (spotAngleY, spotAngleSize);
      double pz = E / sqrt (1. + dxdz*dxdz + dydz*dydz);

      double x0 = G4RandFlat::shoot(spotCenterX-spotSize, spotCenterX+spotSize); 
      double y0 = G4RandFlat::shoot(spotCenterY-spotSize, spotCenterY+spotSize); 
      x0 += -dxdz*15.*CLHEP::cm;
      y0 += -dydz*15.*CLHEP::cm;
      
      G4PrimaryParticle* g4part = new G4PrimaryParticle( g4pd, pz*dxdz, pz*dydz, pz, E );
      
      G4PrimaryVertex* g4vtx = new G4PrimaryVertex( x0, y0, -19.8*CLHEP::cm, 0. ); // xyzt
      g4vtx->SetPrimary(g4part);
      G4Event* g4evt = new G4Event( iev );
      g4evt->AddPrimaryVertex(g4vtx);

      if (!(iev%1000)) cout<<iev<<" generated parameters: "<<" E="<<E<<" x:y="<<x0<<':'<<y0<<" dx:y="<<dxdz<<':'<<dydz<<endl;
      
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

