#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "globals.hh"
#include "G4UserTrackingAction.hh"

class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction();
  virtual ~TrackingAction(){};
  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);
  void AddEdep(G4double energy);
  G4double GetEdep();

private:
  G4double fEdep;
};

#endif