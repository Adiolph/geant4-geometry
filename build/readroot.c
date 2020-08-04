#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>
using namespace std;

void dumpTreeTotxt()
{
    TFile *f = new TFile("OpNovice2.root");    // opens the root file
    TTree *tr = (TTree *)f->Get("tree"); // creates the TTree object
    tr->Scan();                          // prints the content on the screen

    float a, b, c; // create variables of the same type as the branches you want to access

    tr->SetBranchAddress("Electron Spectrum", &a); // for all the TTree branches you need this
    tr->SetBranchAddress("Cerenkov Spectrum", &b);
    tr->SetBranchAddress("Photon Length", &c);

    ofstream myfile;
    myfile.open("root_readout.txt");
    myfile << "ElectronSpectrum CerenkovSpectrum PhotonLength\n";

    for (int i = 0; i < tr->GetEntries(); i++)
    {
        // loop over the tree
        tr->GetEntry(i);
        cout << a << " " << b << " " << c << endl;   //print to the screen
        myfile << a << " " << b << " " << c << "\n"; //write to file
    }
    myfile.close();
}
