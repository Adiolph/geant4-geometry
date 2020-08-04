#include "EMCasCadePhysicsConstructor.hh"

// particles
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

// process
#include "G4GammaConversion.hh"
#include "G4ComptonScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

// helper
#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"

// others
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmParameters.hh"
#include "G4LossTableManager.hh"


EMCasCadePhysicsConstructor::EMCasCadePhysicsConstructor(G4int ver, const G4String &name)
    : G4VPhysicsConstructor("EMCasCadePhysicsConstructor"), verbose(ver)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetApplyCuts(true);
  param->SetGeneralProcessActive(true);
  param->SetDefaults();
  param->SetVerbose(verbose);
  SetPhysicsType(bElectromagnetic);

}

EMCasCadePhysicsConstructor::~EMCasCadePhysicsConstructor()
{
}

void EMCasCadePhysicsConstructor::ConstructParticle()
{
  // gamma
  G4Gamma::Gamma();

  // leptons
  G4Electron::Electron();
  G4Positron::Positron();
}

void EMCasCadePhysicsConstructor::ConstructProcess()
{
  if (verbose > 1)
  {
    G4cout << "### " << GetPhysicsName() << " Construct Processes " << G4endl;
  }
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  // gamma physics
  G4ParticleDefinition* gamma = G4Gamma::Gamma()->Definition();
  G4PhotoElectricEffect* pee = new G4PhotoElectricEffect();
  pee->SetEmModel(new G4LivermorePhotoElectricModel());
  ph->RegisterProcess(new G4GammaConversion(), gamma);
  ph->RegisterProcess(new G4ComptonScattering(), gamma);
  ph->RegisterProcess(pee, gamma);

  // electron physics
  G4ParticleDefinition* electron = G4Electron::Electron()->Definition();
  G4ParticleDefinition* positron = G4Positron::Positron()->Definition();
  G4eIonisation* eioni = new G4eIonisation();
  eioni->SetStepFunction(0.8, 1.0*mm);
  G4eBremsstrahlung* brem = new G4eBremsstrahlung();
  ph->RegisterProcess(eioni, electron);
  ph->RegisterProcess(brem, electron);
  ph->RegisterProcess(eioni, positron);
  ph->RegisterProcess(brem, positron);
  ph->RegisterProcess(new G4eplusAnnihilation(), positron);

}