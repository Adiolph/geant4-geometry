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
/// \file optical/OpNovice2/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fLogicDoms(nullptr),
      fStepLimit(nullptr),
      fUserLimit(nullptr),
      fCheckOverlaps(true)
{
    fNbOfDomX = 4;
    fNbOfDomY = 4;
    fNbOfDomZ = 4;
    fNbOfDomTot = fNbOfDomX * fNbOfDomY * fNbOfDomX;
    fSepDomX = 50. * m;
    fSepDomY = 50. * m;
    fSepDomZ = 50. * m;
    fRadiusDom = 10. * m;
    fLogicDoms = new G4LogicalVolume *[fNbOfDomTot];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete[] fLogicDoms;
    delete fStepLimit;
    delete fUserLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();
    G4double worldSize = 1000 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_WATER");

    //
    // ------------ Generate & Add Material Properties Table ------------
    //
    G4double photonEnergy[] =
        {2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV,
         2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV,
         2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
         2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV,
         2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV,
         3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
         3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV,
         3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV};
    const G4int nEntries = sizeof(photonEnergy) / sizeof(G4double);

    G4double refractiveIndex[] =
        {1.3435, 1.344, 1.3445, 1.345, 1.3455,
         1.346, 1.3465, 1.347, 1.3475, 1.348,
         1.3485, 1.3492, 1.35, 1.3505, 1.351,
         1.3518, 1.3522, 1.3530, 1.3535, 1.354,
         1.3545, 1.355, 1.3555, 1.356, 1.3568,
         1.3572, 1.358, 1.3585, 1.359, 1.3595,
         1.36, 1.3608};
    assert(sizeof(refractiveIndex) == sizeof(photonEnergy));

    G4double absorption[] =
        {3.448 * m, 4.082 * m, 6.329 * m, 9.174 * m, 12.346 * m, 13.889 * m,
         15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
         45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
         52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
         30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
         17.500 * m, 14.500 * m};
    assert(sizeof(absorption) == sizeof(photonEnergy));

    // scattering in water
    G4double energy_water[] = {
        1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV,
        1.65333 * eV, 1.67567 * eV, 1.69863 * eV, 1.72222 * eV,
        1.74647 * eV, 1.77142 * eV, 1.7971 * eV, 1.82352 * eV,
        1.85074 * eV, 1.87878 * eV, 1.90769 * eV, 1.93749 * eV,
        1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
        2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV,
        2.25454 * eV, 2.29629 * eV, 2.33962 * eV, 2.38461 * eV,
        2.43137 * eV, 2.47999 * eV, 2.53061 * eV, 2.58333 * eV,
        2.63829 * eV, 2.69565 * eV, 2.75555 * eV, 2.81817 * eV,
        2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
        3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV,
        3.54285 * eV, 3.64705 * eV, 3.75757 * eV, 3.87499 * eV,
        3.99999 * eV, 4.13332 * eV, 4.27585 * eV, 4.42856 * eV,
        4.59258 * eV, 4.76922 * eV, 4.95999 * eV, 5.16665 * eV,
        5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV};
    const G4int numentries_water = sizeof(energy_water) / sizeof(G4double);

    //assume 100 times larger than the rayleigh scattering for now.
    G4double mie_water[] = {
        167024.4 * m, 158726.7 * m, 150742 * m,
        143062.5 * m, 135680.2 * m, 128587.4 * m,
        121776.3 * m, 115239.5 * m, 108969.5 * m,
        102958.8 * m, 97200.35 * m, 91686.86 * m,
        86411.33 * m, 81366.79 * m, 76546.42 * m,
        71943.46 * m, 67551.29 * m, 63363.36 * m,
        59373.25 * m, 55574.61 * m, 51961.24 * m,
        48527.00 * m, 45265.87 * m, 42171.94 * m,
        39239.39 * m, 36462.50 * m, 33835.68 * m,
        31353.41 * m, 29010.30 * m, 26801.03 * m,
        24720.42 * m, 22763.36 * m, 20924.88 * m,
        19200.07 * m, 17584.16 * m, 16072.45 * m,
        14660.38 * m, 13343.46 * m, 12117.33 * m,
        10977.70 * m, 9920.416 * m, 8941.407 * m,
        8036.711 * m, 7202.470 * m, 6434.927 * m,
        5730.429 * m, 5085.425 * m, 4496.467 * m,
        3960.210 * m, 3473.413 * m, 3032.937 * m,
        2635.746 * m, 2278.907 * m, 1959.588 * m,
        1675.064 * m, 1422.710 * m, 1200.004 * m,
        1004.528 * m, 833.9666 * m, 686.1063 * m};
    assert(sizeof(mie_water) == sizeof(energy_water));

    // construct material properties table
    G4MaterialPropertiesTable *waterMPT = new G4MaterialPropertiesTable();
    waterMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
        ->SetSpline(true);
    waterMPT->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries)
        ->SetSpline(true);
    // gforward, gbackward, forward backward ratio
    G4double mie_water_const[3] = {0.99, 0.99, 0.8};
    waterMPT->AddProperty("MIEHG", energy_water, mie_water, numentries_water)
        ->SetSpline(true);
    waterMPT->AddConstProperty("MIEHG_FORWARD", mie_water_const[0]);
    waterMPT->AddConstProperty("MIEHG_BACKWARD", mie_water_const[1]);
    waterMPT->AddConstProperty("MIEHG_FORWARD_RATIO", mie_water_const[2]);
    waterMPT->DumpTable();

    // set matieral properties table
    worldMat->SetMaterialPropertiesTable(waterMPT);

    // solid volume for the water world
    G4Box *solidWorld =
        new G4Box("World_SW",                                         //its name
                  0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize); //its size
    // logical volume for the water world
    G4LogicalVolume *logicWorld =
        new G4LogicalVolume(solidWorld,  //its solid
                            worldMat,    //its material
                            "World_LV"); //its name
    // physical volume
    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(0,               //no rotation
                          G4ThreeVector(), //at (0,0,0)
                          logicWorld,      //its logical volume
                          "World_PV",      //its name
                          0,               //its mother  volume
                          false,           //no boolean operation
                          0,               //copy number
                          true);           //overlaps checking

    // solid volume for DOM
    G4double dom_radius = 0.2 * m;
    G4Sphere *solidDom =
        new G4Sphere("DOM_SW",
                     0.f, dom_radius, 0.f, 2 * M_PI, 0.f, M_PI);
    // logical volume for the DOM
    G4Material *materialDom = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    for (G4int idx = 0; idx < fNbOfDomTot; idx++)
    {
        G4int nx = static_cast<int>(idx / fNbOfDomY / fNbOfDomZ);
        G4int ny = static_cast<int>((idx - (fNbOfDomY * fNbOfDomZ) * nx) / fNbOfDomZ);
        G4int nz = idx - (fNbOfDomY * fNbOfDomZ) * nx - fNbOfDomZ * ny;
        G4float tx = (nx - (fNbOfDomX - 1.f) / 2.f) * fSepDomX;
        G4float ty = (ny - (fNbOfDomY - 1.f) / 2.f) * fSepDomY;
        G4float tz = (nz - (fNbOfDomZ - 1.f) / 2.f) * fSepDomZ;
        // set up logical volume
        fLogicDoms[idx] =
            new G4LogicalVolume(solidDom, materialDom, "DOM_LV");
        // set up physical volume
        new G4PVPlacement(0,
                          G4ThreeVector(tx, ty, tz),
                          fLogicDoms[idx],
                          "DOM_PV",
                          logicWorld,
                          false,
                          idx,
                          fCheckOverlaps);
    }

    // set user limit
    fUserLimit = new G4UserLimits();
    fUserLimit->SetUserMinEkine(0.25 * MeV);
    logicWorld->SetUserLimits(fUserLimit);

    // return physics volume
    return physWorld;
}