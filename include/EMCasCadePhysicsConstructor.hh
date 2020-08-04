#ifndef EMCasCadePhysicsConstructor_h
#define EMCasCadePhysicsConstructor_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EMCasCadePhysicsConstructor : public G4VPhysicsConstructor
{
public:

  explicit EMCasCadePhysicsConstructor(G4int ver=1, const G4String& name="");

  virtual ~EMCasCadePhysicsConstructor();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif