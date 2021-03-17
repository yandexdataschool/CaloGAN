
#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization(const CaloConfiguration*);
  virtual ~ActionInitialization();
  
  virtual void BuildForMaster() const;
  virtual void Build() const;
  
private:
  const CaloConfiguration* fConfiguration;
};

#endif

