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
/// \file Hadr03.cc
/// \brief Main program of the hadronic/Hadr03 example
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Types.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "SteppingVerbose.hh"
#include "HistoManager.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {

    //detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc,argv);

    //choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    G4int nThreads = G4Threading::G4GetNumberOfCores();
    nThreads = 2; //G4UIcommand::ConvertToInt(argv[2]);
    runManager->SetNumberOfThreads(nThreads);
#else
    //my Verbose output class
    G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    G4RunManager* runManager = new G4RunManager;
#endif

    //set mandatory initialization classes
    DetectorConstruction* det = new DetectorConstruction;
    runManager->SetUserInitialization(det);

    PhysicsList* phys = new PhysicsList;
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new ActionInitialization(det));

    //initialize visualization
    G4VisManager* visManager = nullptr;

    //get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui)  {
        //interactive mode
        visManager = new G4VisExecutive;
        visManager->Initialize();
        ui->SessionStart();
        delete ui;
    }
    else if(argc == 6) //if there are 5 arguments ./exe macro.mac Emin Emax                Estep             NbOfParticle
    {


        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);

        G4double Emin = G4UIcommand::ConvertToDouble(argv[2]);
        G4double Emax = G4UIcommand::ConvertToDouble(argv[3]);
        G4double Estep = G4UIcommand::ConvertToDouble(argv[4]);

        for(double i = Emin; i<=Emax; i+=Estep)
        {
            std::string s_ene = std::to_string(i);
            std::string toReplace (".");

            std::size_t pos = s_ene.find(toReplace);
            s_ene = s_ene.substr(0, pos+3);
            if (pos == std::string::npos){
                /* s_ene = std::to_string(i); */
            }
            else{
                s_ene = s_ene.replace(pos, toReplace.length(), "p");
            }
            G4cout << "Energy post  = " << s_ene << G4endl;

            std::cout << std::setprecision(2);

            G4String energy = "/gun/energy " + std::to_string(i) + " MeV";
            G4String particle = "/run/beamOn " +(G4String)argv[5];

            UImanager->ApplyCommand("/analysis/setFileName HISTO_" + s_ene + "MeV");
            UImanager->ApplyCommand(energy);
            UImanager->ApplyCommand(particle);


        }
    }
    else  {
        //batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    //job termination
    delete visManager;
    delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
