
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
//C++
#include <iostream>
#include <cstdlib>

//ROOT
#include "TROOT.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TApplication.h"

//gtest test
#include "gtest/gtest.h"


#include "material.h"
#include "geometry.h"
#include "CMS.h"
#include "particle.h"
#include "datatypes.h"
#include "Simulator.h"
#include "path.h"
#include "displaygeometry.h"
#include "displaycore.h"
#include "displaypfobjects.h"
#include "TVector3.h"
#include "Edge.h"
#include "PFBlock.h"
#include "PFEvent.h"
#include "PFBlockBuilder.h"
#include "PFReconstructor.h"


extern int run_tests(int argc, char* argv[]);


int main(int argc, char* argv[]){
  
  //int test = run_tests(argc, argv);
  
  // ROOT App to allow graphs to be plotted
  TApplication theApp("App", &argc, argv);
  if (gROOT->IsBatch()) {
    fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
    return 1;
  }
  
  //Create CMS detector and simulator
  CMS CMSDetector;
  Simulator sim = Simulator{CMSDetector};
  
  //Make Some Photons
  for (int i = 1; i<1; i++) {
    PFParticle& photon = sim.addParticle(22, M_PI/2. + 0.025*i, M_PI/2. + 0.3*i, 100 );
    sim.simulatePhoton(photon);
  }
  
  //Make Some Hadrons
  for (int i = 0; i<1; i++) {
    PFParticle& hadron = sim.addParticle(211, M_PI/2. + 0.5*(i + 1) , 0, 40.*(i + 1));
    sim.simulateHadron(hadron);
    
  }
  
  //setup a PFEvent by copying the simulation tracks and cluster (retaining same identifiers)
  // and using  a reference to the history nodes
  PFEvent pfEvent{sim.smearedECALClusters(),
    sim.smearedHCALClusters(),
    sim.smearedTracks(),
    sim.historyNodes()};
  
  
  //Reconstruct
  PFBlockBuilder bBuilder{pfEvent};
  pfEvent.setBlocks(std::move(bBuilder.blocks()));
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();
  
  //TODO try to remove/reduce use of shared_ptrs here.
  Display display = Display({Projection::xy,Projection::yz});
  // All displays
  //Display display = Display({Projection::xy,Projection::yz,Projection::xz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
  std::shared_ptr<GDetector> gdetector(new GDetector(CMSDetector));
  display.addToRegister(gdetector, 0);
  
  //plot clusters
  for (auto& cl : pfEvent.ECALClusters()) {
    std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    display.addToRegister(gcluster,2);
  }
  for (auto& cl :  pfEvent.HCALClusters()) {
    std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    display.addToRegister(gcluster,2);
  }
  for (auto& tr :  pfEvent.tracks()) {
    std::shared_ptr<GTrajectories> gtrack(new GTrajectories(tr.second));
    display.addToRegister(gtrack,2);
  }
  display.draw();
  //TODO uncomment for commandline
  //run theApp.Run();
  
  
  //sim.testing(); //Write lists of connected items
  

  return EXIT_SUCCESS;
}








