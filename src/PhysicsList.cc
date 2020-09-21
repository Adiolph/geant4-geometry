#include "PhysicsList.hh"
#include "EMCasCadePhysicsConstructor.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4IonPhysics.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmParameters.hh"

PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
  SetVerboseLevel(1);
  RegisterPhysics(new EMCasCadePhysicsConstructor());

  // // Default physics
  // RegisterPhysics(new G4DecayPhysics());

  // // EM physics
  // RegisterPhysics(new G4EmStandardPhysics_option1);

  // // Synchroton Radiation & GN Physics
  // // RegisterPhysics(new G4EmExtraPhysics());

  // // Hadron Elastic scattering
  // RegisterPhysics(new G4HadronElasticPhysics());

  // // Hadron Physics
  // RegisterPhysics(new G4HadronPhysicsFTFP_BERT());

  // // Stopping Physics
  // RegisterPhysics(new G4StoppingPhysics());

  // // Ion Physics
  // RegisterPhysics(new G4IonPhysics());

  // // Neutron tracking cut
  // RegisterPhysics(new G4NeutronTrackingCut());

  // // Radioactive decay
  // RegisterPhysics(new G4RadioactiveDecayPhysics());

  // // optical physics
  // G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
  // opticalPhysics->SetMaxNumPhotonsPerStep(400);
  // opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
  // // RegisterPhysics(opticalPhysics);

  G4EmParameters* param = G4EmParameters::Instance();
  param->SetApplyCuts(true);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  G4double cutForGamma = 500 * mm;
  G4double cutForElectron = 0.5 * mm;
  G4double cutForPositron = 0.5 * mm;
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  G4double lowlimit = 10 * GeV;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit, 1000. * TeV);
  // G4VUserPhysicsList::SetCuts();
  DumpCutValuesTable();
}
