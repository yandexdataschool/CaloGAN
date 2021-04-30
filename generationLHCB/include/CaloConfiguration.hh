#ifndef caloconfiguration_hh
#define caloconfiguration_hh 1

#include "G4SystemOfUnits.hh"

class  G4VPhysicalVolume;

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
  int aggregateLayers;
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
    airGap (0.1*mm),
    length (140*mm),
    frontCellLength (0),
    fibreMaterial (QUARTZ),
    absorberMaterial (TUNGSTEN)
  {}

  int modules;
  int cells;
  int fibres;
  double fibreSize;
  double airGap;
  double length;
  double frontCellLength;
  FibreMaterial fibreMaterial;
  AbsorberMaterial absorberMaterial;
};


class CaloConfiguration {
public:

  CaloConfiguration ();
  CaloConfiguration (const ShashlikCaloConfiguration& cfg);
  CaloConfiguration (const SpacalCaloConfiguration& cfg);
  
  bool isShashlik() const {return fIsShashlik;} 
  int modules() const;
  int cells() const;
  int calGranularity () const {return modules() * cells();}
  double moduleSize ()  const {return fModuleSize;}
  double calorSize () const {return moduleSize() * modules();}
  double cellSize () const {return moduleSize() / cells();}
  double calorLength () const;
  double frontCellLength () const;
  
  const G4VPhysicalVolume* absorberPV() const {return fAbsorberPV;}
  const G4VPhysicalVolume* sensitivePV() const {return fSensitivePV;}
  void setPV (const G4VPhysicalVolume* absorberPV, const G4VPhysicalVolume* sensitivePV) {
    if (absorberPV) fAbsorberPV = absorberPV;
    if (sensitivePV) fSensitivePV = sensitivePV;
  }
  
  
  const ShashlikCaloConfiguration& shashlik () const {return fShashlik;}
  const SpacalCaloConfiguration& spacal () const {return fSpacal;}

  bool cherenkovPhotonsSignal () const;
  bool ionizationLossesSignal () const;
  
private:
  bool fIsShashlik;
  ShashlikCaloConfiguration fShashlik;
  SpacalCaloConfiguration fSpacal;
  double fModuleSize;
  const G4VPhysicalVolume* fAbsorberPV;
  const G4VPhysicalVolume* fSensitivePV;
};


#endif
