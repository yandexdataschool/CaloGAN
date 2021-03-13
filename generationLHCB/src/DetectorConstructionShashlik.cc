
#include "DetectorConstructionShashlik.hh"
#include "constants.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstructionShashlik::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstructionShashlik::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionShashlik::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density;
  G4int natoms;
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

  // Scintilator material
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element("Hydrogen", "H" , z=1., a);
  a = 12.01*g/mole;
  G4Element* elC  = new G4Element("Carbon"  ,"C" , z=6., a);
  G4Material* Sci = new G4Material("Scintillator", density = 1.032*g/cm3, 2);
  Sci->AddElement(elC, natoms=9);
  Sci->AddElement(elH, natoms=10);


  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstructionShashlik::DefineVolumes()
{
  // Geometry parameters
  //  G4double moduleSize = 121.2*mm;
  // G4int nofModules = 5;
  // G4int nofLayers = 66;
  // G4double absoThickness = 2.*mm;
  // G4double gapThickness =  4.*mm;
  // G4double calorSizeXY  = nofModules*moduleSize;

  // G4double layerThickness = absoThickness + gapThickness;
  // G4double calorThickness = nofLayers * layerThickness;
  // G4double worldSizeXY = 1.2 * calorSizeXY;
  // G4double worldSizeZ  = 1.2 * calorThickness; 
  
  // Get materials
  G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
  G4Material* absorberMaterial = G4Material::GetMaterial("G4_Pb");
  G4Material* scintillatorMaterial = G4Material::GetMaterial("Scintillator");
  
  if ( ! defaultMaterial || ! absorberMaterial || ! scintillatorMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstructionShashlik::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //
  const double worldScale = 1.2;
  G4VSolid* worldS 
    = new G4Box("World",           // its name
                 calorSizeX*worldScale/2, calorSizeY*worldScale/2, totalLength*worldScale/2); // its size
                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps ());  // checking overlaps 
  
  //                               
  // Calorimeter
  //  
  G4VSolid* calorimeterS
    = new G4Box("Calorimeter",     // its name
                 calorSizeX/2, calorSizeY/2, totalLength/2); // its size
                         
  G4LogicalVolume* calorLV
    = new G4LogicalVolume(
                 calorimeterS,     // its solid
                 defaultMaterial,  // its material
                 "Calorimeter");   // its name
                                   
  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 calorLV,          // its logical volume                         
                 "Calorimeter",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps ());  // checking overlaps 
  
  //                                 
  // Layer
  //
  G4VSolid* layerS 
    = new G4Box("Layer",           // its name
                 calorSizeX/2, calorSizeY/2, layerThickness/2); // its size
                         
  G4LogicalVolume* layerLV
    = new G4LogicalVolume(
                 layerS,           // its solid
                 defaultMaterial,  // its material
                 "Layer");         // its name

  new G4PVReplica(
                 "Layer",          // its name
                 layerLV,          // its logical volume
                 calorLV,          // its mother
                 kZAxis,           // axis of replication
                 nLayers,        // number of replica
                 layerThickness);  // witdth of replica
  
  //                               
  // Absorber
  //
  G4VSolid* absorberS 
    = new G4Box("Absorber",            // its name
                 calorSizeX/2, calorSizeY/2, absorberThickness/2); // its size
                         
  G4LogicalVolume* absorberLV
    = new G4LogicalVolume(
                 absorberS,        // its solid
                 absorberMaterial, // its material
                 "Absorber");          // its name
                                   
  SetAbsorberPV (new G4PVPlacement(
				   0,                // no rotation
				   G4ThreeVector(0., 0., -scintillatorThickness/2), // its position
				   absorberLV,       // its logical volume                         
				   "Absorber",           // its name
				   layerLV,          // its mother  volume
				   false,            // no boolean operation
				   0,                // copy number
				   checkOverlaps ())  // checking overlaps 
		 );
  //                               
  // Scintillator
  //
  G4VSolid* scintillatorS 
    = new G4Box("Scintillator",             // its name
                 calorSizeX/2, calorSizeY/2, scintillatorThickness/2); // its size
                         
  G4LogicalVolume* scintillatorLV
    = new G4LogicalVolume(
                 scintillatorS,             // its solid
                 scintillatorMaterial,      // its material
                 "Scintillator");           // its name
                                   
  SetSensitivePV (new G4PVPlacement(
				    0,                // no rotation
				    G4ThreeVector(0., 0., absorberThickness/2), // its position
				    scintillatorLV,            // its logical volume                         
				    "Scintillator",            // its name
				    layerLV,          // its mother  volume
				    false,            // no boolean operation
				    0,                // copy number
				    checkOverlaps ())  // checking overlaps 
		  );
  //
  // print parameters
  //
  G4cout
    << G4endl 
    << "------------------------------------------------------------" << G4endl
    << "---> The calorimeter is " << nLayers << " layers of: [ "
    << absorberThickness/mm << "mm of " << absorberMaterial->GetName() 
    << " + "
    << scintillatorThickness/mm << "mm of " << scintillatorMaterial->GetName() << " ] " << G4endl
    << "------------------------------------------------------------" << G4endl;
  
  //                                        
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionShashlik::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
