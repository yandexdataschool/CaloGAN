
#ifndef DetectorConstructionSpacal_h
#define DetectorConstructionSpacal_h 1

#include "DetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

/// Fedor Ratnikov

class DetectorConstructionSpacal : public DetectorConstruction
{
  public:
  DetectorConstructionSpacal() : DetectorConstruction () {}
  virtual ~DetectorConstructionSpacal() {}

  public:
    virtual G4VPhysicalVolume* Construct();
     
  private:
    G4VPhysicalVolume* DefineVolumes();
};

#endif

