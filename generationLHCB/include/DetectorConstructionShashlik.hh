
#ifndef DetectorConstructionShashlik_h
#define DetectorConstructionShashlik_h 1

#include "DetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class DetectorConstructionShashlik : public DetectorConstruction
{
  public:
  DetectorConstructionShashlik()
    : DetectorConstruction ()
  {}
  
  virtual ~DetectorConstructionShashlik() {} 

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

     
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger
     
};


#endif

