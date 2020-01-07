
#include "EventAction.hh"
#include "RunData.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iostream>

using namespace std;

EventAction::EventAction()
 : G4UserEventAction()
{}

EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
  G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();

  //  cout<<"EventAction::EndOfEventAction-> Z pz "<<primaryVertex->GetPosition ()[2]<<' '<<primaryParticle->GetMomentum ()[2]<<endl;

  RunData* runData 
    = static_cast<RunData*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  runData->SetParticle (primaryParticle->GetPDGcode (), primaryVertex->GetPosition (), primaryParticle->GetMomentum ());
  runData->FillPerEvent();
}  
