// Fedor Ratnikov

#include "DetectorConstruction.hh"
#include "CaloConfiguration.hh"

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

#include "SpacalMaterials.hh"

const bool checkOverlaps = true;

G4VPhysicalVolume* DetectorConstructionSpacal::Construct()
{
  
  G4Material* airMaterial = SpacalMaterials::Air();
  
  // if      ( mat == 1 ) AbMaterial = MyMaterials::Brass () ;
  // else if ( mat == 2 ) AbMaterial = MyMaterials::Tungsten () ;
  // else if ( mat == 3 ) AbMaterial = MyMaterials::Lead () ;
  // else if ( mat == 4 ) AbMaterial = MyMaterials::Iron () ;
  // else if ( mat == 5 ) AbMaterial = MyMaterials::Aluminium () ;
  // else if ( mat == 6 ) AbMaterial = MyMaterials::CopperTungstenAlloy(W_fraction) ;
  // else if ( mat == 7 ) AbMaterial = MyMaterials::PureTungsten1() ;
  // else if ( mat == 8 ) AbMaterial = MyMaterials::PureTungsten2() ;
  // else if ( mat == 9 ) AbMaterial = MyMaterials::AirKiller() ;
  // else if ( mat == 10 ) AbMaterial = MyMaterials::Air() ;
  // else if ( mat == 11 ) AbMaterial = MyMaterials::LAPPD_Average() ;
  // else if ( mat == 12 ) AbMaterial = MyMaterials::StainlessSteel() ;
  // else if ( mat == 13 ) AbMaterial = MyMaterials::GarthTypographicAlloy() ;

  //G4Material* absorberMaterial = SpacalMaterials::PureTungsten2();

  // if      ( mat == 1 ) CrystalMaterial = MyMaterials::Quartz () ;
  // else if ( mat == 2 ) CrystalMaterial = MyMaterials::SiO2_Ce () ;
  // else if ( mat == 3 ) CrystalMaterial = MyMaterials::DSB_Ce () ;
  // else if ( mat == 4 ) CrystalMaterial = MyMaterials::LuAG_Ce () ;
  // else if ( mat == 5 ) CrystalMaterial = MyMaterials::YAG_Ce (user_lightyield,scaleFactor) ;
  // else if ( mat == 6 ) CrystalMaterial = MyMaterials::GAGG_Ce_Mg(user_lightyield,scaleFactor) ;
  // else if ( mat == 7 ) CrystalMaterial = MyMaterials::Water() ;
  // else if ( mat == 8 ) CrystalMaterial = MyMaterials::GFAG(user_lightyield,scaleFactor) ;
  // else if ( mat == 9 ) CrystalMaterial = MyMaterials::GAGG_very_fast(user_lightyield,scaleFactor) ;
  // else if ( mat == 10 ) CrystalMaterial = MyMaterials::GYAGG(user_lightyield,scaleFactor) ;
  // else if ( mat == 11 ) CrystalMaterial = MyMaterials::GAGG_slow(user_lightyield,scaleFactor) ;
  // else if ( mat == 12 ) CrystalMaterial = MyMaterials::Polystyrene(user_lightyield,scaleFactor) ;
  // else if ( mat == 13 ) CrystalMaterial = MyMaterials::Lead_crystal() ;
  // else if ( mat == 14 ) CrystalMaterial = MyMaterials::PureTungsten2_crystal() ;
  // else if ( mat == 15 ) CrystalMaterial = MyMaterials::Al_crystal() ;
  // else if ( mat == 16 ) CrystalMaterial = MyMaterials::Cu_crystal() ;
  // else if ( mat == 17 ) CrystalMaterial = MyMaterials::GAGG_ILM(user_lightyield,scaleFactor) ;

  // Scintilator material
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element("Hydrogen", "H" , z=1., a);
  a = 12.01*g/mole;
  G4Element* elC  = new G4Element("Carbon"  ,"C" , z=6., a);
  G4Material* Sci = new G4Material("Scintillator", density = 1.032*g/cm3, 2);
  Sci->AddElement(elC, natoms=9);
  Sci->AddElement(elH, natoms=10);


  

  G4Material* absorberMaterial = SpacalMaterials::Lead();
  G4Material* sensitiveMaterial = Sci;
  if (config->isSpacal()) {
    switch (config->spacal().absorberMaterial) {
    case TUNGSTEN:
      absorberMaterial = SpacalMaterials::PureTungsten2();
      break;
    }
    switch (config->spacal().fibreMaterial) {
    case QUARTZ:
      sensitiveMaterial = SpacalMaterials::Quartz();
      break;
    case GAGG:
      //      sensitiveMaterial = SpacalMaterials::Quartz();
      break;
    case POLYSTYRENE:
      //      sensitiveMaterial = SpacalMaterials::Quartz();
      break;
    }
    
  }

  const G4VPhysicalVolume* absorberPV = 0;
  const G4VPhysicalVolume* sensitivePV = 0;
  
  const double calorLength = config->calorLength();
  const double calorWidth = config->calorSize();

  //     
  // World
  //
  const double worldScale = 1.05;
  G4VSolid* worldS 
    = new G4Box("World",           // its name
		calorWidth*worldScale/2, calorWidth*worldScale/2,  calorLength*worldScale/2); // its size
                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 airMaterial,  // its material
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
                 checkOverlaps);  // checking overlaps 
  
  //                               
  // Calorimeter  
  //
  G4LogicalVolume* calorLV
    = new G4LogicalVolume(
			  new G4Box ("Calorimeter", calorWidth/2, calorWidth/2,  calorLength/2);
			  absorberMaterial,  // its material
			  "Calorimeter");   // its name
  
  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 calorLV,          // its logical volume                         
                 "calorimeter",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 checkOverlaps);  // checking overlaps

  // Calorimeter structure
  //
  if (config->isShashlik ()) { // Shashlik calo
    const double absorberThickness = config->shashlik.absorberDepth;
    const double scintillatorThickness = config->shashlik.sciDepth;
    const double layerThickness = absorberThickness + scintillatorThickness;
    int nLayers = config->shashlik.nLayers;
    //                                 
    // Layer
    //
    G4VSolid* layerS 
      = new G4Box("Layer",           // its name
		  calorWidth/2, calorWidth/2, /2); // its size
    
    G4LogicalVolume* layerLV
      = new G4LogicalVolume(
			    new G4Box ("Layer", calorWidth/2, calorWidth/2, layerThickness/2),           // its solid
			    absorberMaterial,  // its material
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
    G4LogicalVolume* absorberLV
      = new G4LogicalVolume(
			    new G4Box ("Absorber", calorWidth, calorWidth/2, absorberThickness/2),        // its solid
			    absorberMaterial, // its material
			    "Absorber");          // its name
    
    absorberPV = new G4PVPlacement(
				   0,                // no rotation
				   G4ThreeVector(0., 0., -scintillatorThickness/2), // its position
				   absorberLV,       // its logical volume                         
				   "Absorber",           // its name
				   layerLV,          // its mother  volume
				   false,            // no boolean operation
				   0,                // copy number
				   checkOverlaps);  // checking overlaps 
  //                               
  // Scintillator
  //
  G4LogicalVolume* scintillatorLV
    = new G4LogicalVolume(
                 new G4Box("Scintillator",             // its name
			   calorWidth/2, calorWidth/2, scintillatorThickness/2),
                 scintillatorMaterial,      // its material
                 "Scintillator");           // its name
                                   
  sensitivePV = new G4PVPlacement(
				  0,                // no rotation
				  G4ThreeVector(0., 0., absorberThickness/2), // its position
				  scintillatorLV,            // its logical volume                         
				  "Scintillator",            // its name
				  layerLV,          // its mother  volume
				  false,            // no boolean operation
				  0,                // copy number
				  checkOverlaps);  // checking overlaps 
  }
  else { // spacal calo
    const double spacalPitchSize = config->calorSize() / config->spacal.fibers;
    //                               
    // Calorimeter Y slice 
    //  
    G4LogicalVolume* calorSliceLV
      = new G4LogicalVolume(
			    new G4Box ("CalorimeterSlice", spacalPitchSize/2, calorWidth/2, calorLength/2),
			    absorberMaterial,  // its material
			    "CalorimeterSlice");   // its name
    
    new G4PVReplica(
		    "CalorimeterSlice",          // its name
		    calorSliceLV,          // its logical volume
		    calorLV,          // its mother
		    kXAxis,           // axis of replication
		    config->spacal.fibers * config->modules(), // number of replica
		    spacalPitchSize);  // witdth of replica
    
    //                               
    // Calorimeter cell 
    //  
    
    G4LogicalVolume* calorCellLV
      = new G4LogicalVolume(
			    new G4Box("CalorimeterCell", spacalPitchSize/2, spacalPitchSize/2, calorLength/2),
			    absorberMaterial,  // its material
			    "CalorimeterCell");   // its name
    
    absorberPV = new G4PVReplica(
				 "CalorimeterCell",          // its name
				 calorCellLV,          // its logical volume
				 calorSliceLV,          // its mother
				 kYAxis,           // axis of replication
				 config->spacal.fibers * config->modules(), // number of replica
				 spacalPitchSize);  // witdth of replica
    
    
    //                               
    // Fibre 
    //
    
    G4LogicalVolume* fibreLV =
      new G4LogicalVolume ( new G4Box("fibre", config->spacal.fibreSize/2, config->spacal.fibreSize/2, calorLength/2),
			    fibreMaterial,
			    "fibre");
    sensitivePV = new G4PVPlacement(
				    0,                // no rotation
				    G4ThreeVector(),  // at (0,0,0)
				    fibreLV,          // its logical volume                         
				    "fibre",    // its name
				    calorCellLV,          // its mother  volume
				    false,            // no boolean operation
				    0,                // copy number
				    checkOverlaps);  // checking overlaps
  }
  config->setPV (absorberPV, sensitivePV);
  //
  // Always return the physical World
  //
  return worldPV;
}

