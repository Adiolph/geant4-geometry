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
/// \file optical/OpNovice2/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "G4Timer.hh"
#include "G4UnitsTable.hh"

#include "RunAction.hh"
#include "Analysis.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(PrimaryGeneratorAction *prim)
    : G4UserRunAction(),
      fTimer(nullptr),
      fAnalysisManager(nullptr),
      fPrimary(prim)
{
  fTimer = new G4Timer;
  fAnalysisManager = new Analysis();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
  delete fAnalysisManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run *aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (fPrimary)
  {
    G4ParticleDefinition *particle =
        fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    G4cout << "Particle name: " << particle->GetParticleName() << G4endl;
    G4cout << "Particle energy: " << energy << G4endl;
  }

  //histograms
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsActive())
  {
    analysisManager->OpenFile();
  }

  fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run *aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;

  // save histograms
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

  if (analysisManager->GetH1(1))
  {
    G4cout << G4endl << " ----> print histograms statistic ";
    // if (isMaster)
    {
      G4cout << "for the entire run " << G4endl << G4endl;
    }
    // else
    {
      G4cout << "for the local thread " << G4endl << G4endl;
    }

    // G4cout << " Number of Cerenkov photons:"
    //        << analysisManager->GetH1(1)->get_bins()
    //        << " mean energy = "
    //        << G4BestUnit(analysisManager->GetH1(1)->rms(), "Energy") << G4endl;
  }

  if (analysisManager->IsActive())
  {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
