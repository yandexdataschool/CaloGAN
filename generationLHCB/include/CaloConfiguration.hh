#ifndef caloconfiguration_hh
#define caloconfiguration_hh 1

#include "G4SystemOfUnits.hh"

class  G4VPhysicalVolume* absorberPV;

class ShashlikCaloConfiguration {
public:
  ShashlikCaloConfiguration () :
    modules (5),
    cells (6),
    sciDepth (4*mm),
    absorberDepth (2*mm),
    nLayers (66),
    aggregateLayers (nLayers),
    frontCellLength (0)

  {}
  
  int modules;
  int cells;
  double sciDepth;
  double absorberDepth;
  int nLayers;
  int aggregateLayersFibreMaterial
  double frontCellLength;
};

class SpacalCaloConfiguration {
public:
  enum FibreMaterial {
    QUARTZ,
    GAGG,
    POLYSTYRENE
  };
  enum AbsorberMaterial {
    LEAD,
    TUNGSTEN
  };
  
  SpacalCaloConfiguration ():
    modules (5),
    cells (8),
    fibres (72),
    fibreSize (1*mm),
    length (140mm),
    frontCellLength (0),
    fibreMaterial (QUARTZ)
    absorberMaterial (TUNGSTEN)
  {}

  int modules;
  int cells;
  int fibres;
  double fibreSize;
  double length;
  double frontCellLength;
  FibreMaterial fibreMaterial
  AbsorberMaterial absorberMaterial
  
};


class CaloConfiguration {
public:

  CaloConfiguration ();
  CaloConfiguration (const ShashlikCaloConfiguration& cfg);
  CaloConfiguration (const SpacalCaloConfiguration& cfg);
  
  bool isShashlik() const {return isShashlik;} 
  int modules() const;
  int cells() const;
  int calGranularity () const {return modules() * cells();}
  double moduleSize ()  const {return moduleSize;}
  double calorSize () const {return moduleSize() * modules();}
  double cellSize () const {return moduleSize() / cells();}
  double calorLength () const;
  double frontCellLength () const;
  
  const G4VPhysicalVolume* absorberPV() const {return absorberPV;}
  const G4VPhysicalVolume* sensitivePV() const {return absorberPV;}
  void setPV (const G4VPhysicalVolume* theAbsorberPV, const G4VPhysicalVolume* theSensitivePV) {
    if (theAbsorberPV) absorberPV = theAbsorberPV;
    if (theSensitivePV) sensitivePV = theSensitivePV;
  }
  
  
  const ShashlikCaloConfiguration& shashlik () const {return shashlik;}
  const SpacalCaloConfiguration& spacal () const {return spacal;}
  
private:
  bool isShashlik;
  ShashlikCaloConfiguration shashlik;
  SpacalCaloConfiguration spacal;
  double moduleSize;
  const G4VPhysicalVolume* absorberPV;
  const G4VPhysicalVolume* sensitivePV;
};


  
  int modules;
  int cells;
  
  int modulesX;
  int moduleCells = 8;
  double moduleSize = 121.2 * mm;
  double ShashlikScintillatorThickness = 4. * mm;
  double shashlikAbsorberThickness = 2. * mm;
  int shashlikLayers = 66;
  int aggregateLayers = 66;

  double spacalTotalLength = 140. * mm;
  int spacalModuleFibresXY = 72;
  double spacalFibreSize = 1*mm;
  double spacalGapSize = spacalFibreSize + 0.1*2*mm;

};

#endif
