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
/// \file optical/OpNovice2/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::Analysis()
    : fFileName("OpNovice2")
{
  fAnalysisManager = G4AnalysisManager::Instance();
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::~Analysis()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  fAnalysisManager->SetFileName(fFileName);
  fAnalysisManager->SetVerboseLevel(1);
  fAnalysisManager->SetActivation(true); // enable inactivation of histograms

  // Creating histograms
  //
  // Define histogram indices, titles
  G4int maxHisto = 3;
  G4String name[] = {
      "ElectronSpectrum", // 0
      "CerenkovSpectrum", // 1
      "PhotonLength"      // 2
  };
  G4String title[] = {
      "Electron Spectrum", // 0
      "Cerenkov Spectrum", // 1
      "Photon Length"      // 2
  };
  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;
  // Create all histograms
  for (G4int k = 0; k < maxHisto; ++k)
  {
    G4int ih = fAnalysisManager->CreateH1(name[k], title[k], nbins, vmin, vmax);
    fAnalysisManager->SetH1Activation(ih, true);
  }

  // Creating ntuple for sercondaries
  fAnalysisManager->CreateNtuple("Secondary", "SecondaryTable");
  fAnalysisManager->CreateNtupleSColumn("Name");
  fAnalysisManager->CreateNtupleDColumn("x0");
  fAnalysisManager->CreateNtupleDColumn("y0");
  fAnalysisManager->CreateNtupleDColumn("z0");
  fAnalysisManager->CreateNtupleDColumn("t0");
  fAnalysisManager->CreateNtupleDColumn("nx");
  fAnalysisManager->CreateNtupleDColumn("ny");
  fAnalysisManager->CreateNtupleDColumn("nz");
  fAnalysisManager->CreateNtupleDColumn("E0");
  fAnalysisManager->FinishNtuple();

  // creating ntuple for tracks
  fAnalysisManager->CreateNtuple("Track", "TrackTable");
  fAnalysisManager->CreateNtupleSColumn("Name");
  fAnalysisManager->CreateNtupleDColumn("x0");
  fAnalysisManager->CreateNtupleDColumn("y0");
  fAnalysisManager->CreateNtupleDColumn("z0");
  fAnalysisManager->CreateNtupleDColumn("t0");
  fAnalysisManager->CreateNtupleDColumn("nx");
  fAnalysisManager->CreateNtupleDColumn("ny");
  fAnalysisManager->CreateNtupleDColumn("nz");
  fAnalysisManager->CreateNtupleDColumn("Edep");
  fAnalysisManager->CreateNtupleDColumn("len");
  fAnalysisManager->FinishNtuple();
}

void Analysis_Method::RecordSecondary(const G4Track *sec)
{
  G4AnalysisManager *fAnalysisManager = G4AnalysisManager::Instance();
  G4String name = sec->GetDefinition()->GetParticleName();
  CLHEP::Hep3Vector pos = sec->GetPosition();
  CLHEP::Hep3Vector dir = sec->GetMomentumDirection();
  fAnalysisManager->FillNtupleSColumn(0, 0, name);
  fAnalysisManager->FillNtupleDColumn(0, 1, pos.x() / m);
  fAnalysisManager->FillNtupleDColumn(0, 2, pos.y() / m);
  fAnalysisManager->FillNtupleDColumn(0, 3, pos.z() / m);
  fAnalysisManager->FillNtupleDColumn(0, 4, sec->GetGlobalTime());
  fAnalysisManager->FillNtupleDColumn(0, 5, dir.x());
  fAnalysisManager->FillNtupleDColumn(0, 6, dir.y());
  fAnalysisManager->FillNtupleDColumn(0, 7, dir.z());
  fAnalysisManager->FillNtupleDColumn(0, 8, sec->GetKineticEnergy() / GeV);
  fAnalysisManager->AddNtupleRow(0);
}

void Analysis_Method::RecordTrack(const G4Track *track, G4double Edep)
{
  G4AnalysisManager *fAnalysisManager = G4AnalysisManager::Instance();
  G4String name = track->GetDefinition()->GetParticleName();
  CLHEP::Hep3Vector pos = track->GetPosition();
  CLHEP::Hep3Vector dir = track->GetMomentumDirection();
  fAnalysisManager->FillNtupleSColumn(1, 0, name);
  fAnalysisManager->FillNtupleDColumn(1, 1, pos.x() / m);
  fAnalysisManager->FillNtupleDColumn(1, 2, pos.y() / m);
  fAnalysisManager->FillNtupleDColumn(1, 3, pos.z() / m);
  fAnalysisManager->FillNtupleDColumn(1, 4, track->GetGlobalTime());
  fAnalysisManager->FillNtupleDColumn(1, 5, dir.x());
  fAnalysisManager->FillNtupleDColumn(1, 6, dir.y());
  fAnalysisManager->FillNtupleDColumn(1, 7, dir.z());
  fAnalysisManager->FillNtupleDColumn(1, 8, Edep / GeV);
  fAnalysisManager->FillNtupleDColumn(1, 9, track->GetKineticEnergy() / m);
  fAnalysisManager->AddNtupleRow(1);
}