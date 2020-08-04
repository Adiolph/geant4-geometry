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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::Analysis()
    : fFileName("OpNovice2")
{
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
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true); // enable inactivation of histograms

  // Creating histograms
  //
  // Define histogram indices, titles
  G4cout << "Initial Analysis! " << G4endl;
  G4int maxHisto = 3;
  G4String name[] = {
      "Electron Spectrum", // 0
      "Cerenkov Spectrum", // 1
      "Photon Length"      // 2
  };
  G4String title[] = {
      "Electron Spectrum", // 0
      "Cerenkov Spectrum", // 1
      "Photon Length"      // 2
  };
  G4cout << "Initial Analysis TITLE! " << G4endl;
  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;
  // Create all histograms
  for (G4int k = 0; k < maxHisto; ++k)
  {
    G4int ih = analysisManager->CreateH1(name[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, true);
  }
  G4cout << "Initial Analysis Succeed! " << G4endl;

  // // Creating ntuple
  // //
  // analysisManager->CreateNtuple("Optical", "Spectrums");
  // analysisManager->CreateNtupleDColumn("Electron Spectrum");
  // analysisManager->CreateNtupleDColumn("Cerenkov Spectrum");
  // analysisManager->CreateNtupleDColumn("Photon Length");
  // analysisManager->FinishNtuple();
}
