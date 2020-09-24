//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file optical/OpNovice2/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
//#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Electron.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SteppingAction::SteppingAction(TrackingAction* trackingAction)
    : G4UserSteppingAction(),
      fTrackingAction(trackingAction),
      fVerbose(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SteppingAction::UserSteppingAction(const G4Step *step)
{
  // get particle defination
  static G4ParticleDefinition *opticalphoton =
      G4OpticalPhoton::OpticalPhotonDefinition();
  static G4ParticleDefinition *electron =
      G4Electron::ElectronDefinition();
  G4AnalysisManager *analysisManger = G4AnalysisManager::Instance();

  // add deposite energy
  fTrackingAction->AddEdep(step->GetTotalEnergyDeposit());
  G4cout << "StepAction: energy deposit in this step: " << step->GetTotalEnergyDeposit() << G4endl;

  G4Track *track = step->GetTrack();
  G4String particleName = track->GetParticleDefinition()->GetParticleName();

  if (particleName != "opticalphoton")
  { // particle != opticalphoton
    // print how many Cerenkov and scint photons produced this step
    // this demonstrates use of GetNumPhotons()
    auto proc_man = track->GetParticleDefinition()->GetProcessManager();
    G4int n_proc = proc_man->GetPostStepProcessVector()->entries();
    G4ProcessVector *proc_vec = proc_man->GetPostStepProcessVector(typeDoIt);

    G4int n_scint = 0;
    G4int n_cer = 0;
    for (G4int i = 0; i < n_proc; ++i)
    {
      // G4cout << "fProcess Name: " << (*proc_vec)[i]->GetProcessName() << G4endl;
      if ((*proc_vec)[i]->GetProcessName().compare("Cerenkov") == 0)
      {
        auto cer = (G4Cerenkov *)(*proc_vec)[i];
        n_cer = cer->GetNumPhotons();
      }
      else if ((*proc_vec)[i]->GetProcessName().compare("Scintillation") == 0)
      {
        auto scint = (G4Scintillation *)(*proc_vec)[i];
        n_scint = scint->GetNumPhotons();
      }
    }
    if (fVerbose > 0)
    {
      if (n_cer > 0 || n_scint > 0)
      {
        G4cout << "In this step, " << n_cer
               << " Cerenkov and " << G4endl;
      }
    }

    // loop over secondaries, create statistics
    auto secondaries =
        step->GetSecondaryInCurrentStep();

    for (auto sec : *secondaries)
    {
      // processing for low energy particles
      if (sec->GetKineticEnergy() < 1*GeV)  // low energy particle are disregarded
      {
        G4cout << "StepAction: disregarded secondary particle name: " << sec->GetDefinition()->GetParticleName()
         << " energy: " << sec->GetKineticEnergy() << G4endl;
        fTrackingAction->AddEdep(sec->GetKineticEnergy());
      }
      else if(sec->GetKineticEnergy() < 100*GeV)  // record secondary info
      {
        G4cout << "StepAction: record secondary particle name: " << sec->GetDefinition()->GetParticleName()
         << " energy: " << sec->GetKineticEnergy() << G4endl;
        Analysis_Method::RecordSecondary(sec);
      }

      // record spectrum
      if (sec->GetDynamicParticle()->GetParticleDefinition() == electron)
      {
        G4double energy = sec->GetKineticEnergy();
        analysisManger->FillH1(0, energy);
      }
      else if (sec->GetDynamicParticle()->GetParticleDefinition() == opticalphoton)
      {
        if (sec->GetCreatorProcess()->GetProcessName().compare("Cerenkov") == 0 and sec->GetParentID() == 1)
        {
          G4double energy = sec->GetKineticEnergy();
          analysisManger->FillH1(1, energy);
          G4double length = sec->GetTrackLength();
          analysisManger->FillH1(2, length);
        }
      }
    }
  }
  if (track->GetKineticEnergy() < 100*GeV)
  {
    G4cout << "StepAction: energy of track below 100 GeV, record and terminate track, particle name: "
     << track->GetDefinition()->GetParticleName()
     << " energy: " << track->GetKineticEnergy()
     << " initial kin: " << track->GetVertexKineticEnergy() 
     << " deposition eng: " << fTrackingAction->GetEdep() << G4endl;
    Analysis_Method::RecordTrack(track, fTrackingAction->GetEdep());
    Analysis_Method::RecordSecondary(track);
    track->SetTrackStatus(fStopAndKill);
  }
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
