
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;
class CaloConfiguration;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction(CaloConfiguration& theConfig)
    : G4VUserDetectorConstruction(),
      config(&theConfig)
  {}
  
  virtual ~DetectorConstruction() {}
  
public:
  virtual G4VPhysicalVolume* Construct();
private:
  CaloConfiguration* config;
};

#endif

