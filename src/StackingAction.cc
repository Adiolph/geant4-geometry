#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

StackingAction::StackingAction()
{
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track *track)
{
  if (track->GetParentID() == 0)
    return fUrgent;
  if (track->GetKineticEnergy() < 100 * GeV)
  {
    G4cout << "StackingAction: kill secondary particle name: " 
           << track->GetDefinition()->GetParticleName()
           << " energy: " << track->GetKineticEnergy() << G4endl;
    return fKill;
  }
  else
    return fUrgent;
}