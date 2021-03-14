

#include "DetectorConstructionSpacal.hh"
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

#include "SpacalMaterials.hh"

// Fedor Ratnikov

G4VPhysicalVolume* DetectorConstructionSpacal::Construct()
{
  // Define volumes
  return DefineVolumes();
}


G4VPhysicalVolume* DetectorConstructionSpacal::DefineVolumes()
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
  //  G4Material* absorberMaterial = SpacalMaterials::Lead();
  G4Material* absorberMaterial = SpacalMaterials::PureTungsten2();

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
  G4Material* fibreMaterial = SpacalMaterials::Quartz ();

  G4Material* gapMaterial =   airMaterial;

  
  //     
  // World
  //
  const double worldScale = 1.2;
  G4VSolid* worldS 
    = new G4Box("World",           // its name
                 calorSizeX*worldScale/2, calorSizeY*worldScale/2,  spacalTotalLength*worldScale/2); // its size
                         
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
                 checkOverlaps ());  // checking overlaps 
  
  //                               
  // Calorimeter  
  //  
    G4LogicalVolume* calorLV
    = new G4LogicalVolume(
			  new G4Box ("Calorimeter", calorSizeX/2, calorSizeY/2, spacalTotalLength/2),
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
                 checkOverlaps ());  // checking overlaps 
  //                               
  // Calorimeter Y slice 
  //  
  G4LogicalVolume* calorSliceLV
    = new G4LogicalVolume(
			  new G4Box ("CalorimeterSlice", spacalPitchSize/2, calorSizeY/2, spacalTotalLength/2),
			  absorberMaterial,  // its material
			  "CalorimeterSlice");   // its name
  
  new G4PVReplica(
		  "CalorimeterSlice",          // its name
		  calorSliceLV,          // its logical volume
		  calorLV,          // its mother
		  kXAxis,           // axis of replication
		  spacalModuleFibresXY * modulesX, // number of replica
		  spacalPitchSize);  // witdth of replica

  //                               
  // Calorimeter cell 
  //  

  G4LogicalVolume* calorCellLV
    = new G4LogicalVolume(
			  new G4Box("CalorimeterCell", spacalPitchSize/2, spacalPitchSize/2, spacalTotalLength/2),
			  absorberMaterial,  // its material
			  "CalorimeterCell");   // its name
  
  SetAbsorberPV (new G4PVReplica(
				 "CalorimeterCell",          // its name
				 calorCellLV,          // its logical volume
				 calorSliceLV,          // its mother
				 kYAxis,           // axis of replication
				 spacalModuleFibresXY * modulesX, // number of replica
				 spacalPitchSize)  // witdth of replica
		 );
  // //                               
  // // Fibre gap 
  // //  
  // G4LogicalVolume* gapLV =
  //   new G4LogicalVolume ( new G4Box("gap", spacalGapSize/2, spacalGapSize/2, spacalTotalLength/2),
  // 			  gapMaterial,
  // 			  "gap");
  
  // new G4PVPlacement(
  //                0,                // no rotation
  //                G4ThreeVector(),  // at (0,0,0)
  //                gapLV,          // its logical volume                         
  //                "gap",    // its name
  //                calorCellLV,          // its mother  volume
  //                false,            // no boolean operation
  //                0,                // copy number
  //                checkOverlaps ());  // checking overlaps 

  // //                               
  // // Fibre 
  // //  
  // G4LogicalVolume* fibreLV =
  //   new G4LogicalVolume ( new G4Box("fibre", spacalFibreSize/2, spacalGapSize/2, spacalTotalLength/2),
  // 			  fibreMaterial,
  // 			  "fibre");
                                   
  // SetSensitivePV (new G4PVPlacement(
  // 				    0,                // no rotation
  // 				    G4ThreeVector(),  // at (0,0,0)
  // 				    fibreLV,          // its logical volume                         
  // 				    "fibre",    // its name
  // 				    gapLV,          // its mother  volume
  // 				    false,            // no boolean operation
  // 				    0,                // copy number
  // 				    checkOverlaps ())  // checking overlaps
  // 		  );
  //                               
  // Fibre 
  //  
  G4LogicalVolume* fibreLV =
    new G4LogicalVolume ( new G4Box("fibre", spacalFibreSize/2, spacalGapSize/2, spacalTotalLength/2),
			  fibreMaterial,
			  "fibre");
                                   
  SetSensitivePV (new G4PVPlacement(
				    0,                // no rotation
				    G4ThreeVector(),  // at (0,0,0)
				    fibreLV,          // its logical volume                         
				    "fibre",    // its name
				    calorCellLV,          // its mother  volume
				    false,            // no boolean operation
				    0,                // copy number
				    checkOverlaps ())  // checking overlaps
		  );

  //
  // print parameters
  //

  // G4cout
  //   << G4endl 
  //   << "------------------------------------------------------------" << G4endl
  //   << "---> The calorimeter is " << nLayers << " layers of: [ "
  //   << absorberThickness/mm << "mm of " << absorberMaterial->GetName() 
  //   << " + "
  //   << scintillatorThickness/mm << "mm of " << scintillatorMaterial->GetName() << " ] " << G4endl
  //   << "------------------------------------------------------------" << G4endl;
  //
  // Always return the physical World
  //
  return worldPV;
}

