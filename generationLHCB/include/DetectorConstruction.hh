
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fAbsorberPV (0),
      fSensitivePV (0),
      fCheckOverlaps (true)
  {}
  
  virtual ~DetectorConstruction() {}
  
public:
  virtual G4VPhysicalVolume* Construct() = 0;

  const G4VPhysicalVolume* GetAbsorberPV() const {return fAbsorberPV;} 
  const G4VPhysicalVolume* GetSensitivePV() const {return fSensitivePV;}
  G4bool checkOverlaps () const {return fCheckOverlaps;}
  
protected:
  void SetAbsorberPV (const G4VPhysicalVolume* pv) {fAbsorberPV = pv;}
  void SetSensitivePV (const G4VPhysicalVolume* pv) {fSensitivePV = pv;}
  void SetCheckOverlap (G4bool check) {fCheckOverlaps = check;}

private:
  const G4VPhysicalVolume*   fAbsorberPV; 
  const G4VPhysicalVolume*   fSensitivePV;
  G4bool  fCheckOverlaps; 
};

#endif

