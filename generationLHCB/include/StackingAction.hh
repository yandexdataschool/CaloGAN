// F.Ratnikov
#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"

class G4Track;
class DetectorConstruction;



class StackingAction : public G4UserStackingAction {
public:
  StackingAction(const DetectorConstruction* detectorConstruction) :  fDetConstruction (detectorConstruction) {}
  virtual ~StackingAction();
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
//    Reply G4ClassificationOfNewTrack determined by the
//  newly coming G4Track.
//
//    enum G4ClassificationOfNewTrack
//    {
//      fUrgent,    // put into the urgent stack
//      fWaiting,   // put into the waiting stack
//      fPostpone,  // postpone to the next event
//      fKill       // kill without stacking
//    };
//
//    The parent_ID of the track indicates the origin of it.
//                
//    G4int parent_ID = aTrack->get_parentID();
//
//      parent_ID = 0 : primary particle
//                > 0 : secondary particle
//                < 0 : postponed from the previous event
//
  
private:
  const DetectorConstruction* fDetConstruction;
};
 
#endif
