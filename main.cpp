
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "TApplication.h"
#include "TROOT.h"
#include "gtest/gtest.h"

#include "CMS.h"
#include "PFEventDisplay.h"
#include "displaypfobjects.h"
#include "Simulator.h"
#include "PFBlockBuilder.h"
#include "PFEvent.h"
#include "PFParticle.h"
#include "PFReconstructor.h"
#include "Cluster.h"
#include "Track.h"
#include "Path.h"
#include "random.h"

#include "AliceDisplay.h"
#include "Id.h"

//extern int run_tests(int argc, char* argv[]);
using namespace papas;
int main(int argc, char* argv[]) {
  


  randomgen::RandExponential rexp{49.3};
  rexp.setSeed(100);
  std::cout << rexp.next() << rexp.next()<<std::endl;;
  
  
  
  // Create CMS detector and simulator
  CMS CMSDetector;
  Simulator sim = Simulator{CMSDetector};

  // Make Some Photons
  for (int i = 1; i < 2; i++) {
    PFParticle& photon = sim.addParticle(22, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
    sim.simulatePhoton(photon);
  }

  // Make Some Hadrons
  for (int i = 1; i < 2; i++) {
    PFParticle& hadron = sim.addParticle(211, M_PI / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
    sim.simulateHadron(hadron);
  }

  // setup a PFEvent by copying the simulation tracks and cluster (retaining same identifiers)
  // and using a reference to the history nodes
  //PFEvent pfEvent{sim.smearedEcalClusters(), sim.smearedHcalClusters(), sim.smearedTracks(), sim.historyNodes()};
  PFEvent pfEvent{sim}; //for python test
  
  std::cout<<pfEvent<< std::endl;

  // Reconstruct
  PFBlockBuilder bBuilder{pfEvent};
  //pfEvent.setBlocks(std::move(bBuilder.blocks()));
  pfEvent.setBlocks(bBuilder);//for python
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();

  // Now move on to Displaying results

  PFApp myApp{};
  myApp.display(pfEvent, CMSDetector);
  
    //myApp.run();
  return 0;
  
  
  // ROOT App to allow graphs to be plotted
  TApplication theApp("App", &argc, argv);
  if (gROOT->IsBatch()) {
    fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
    return 1;
  }

  // All displays
  // Display display = Display({Projection::xy,Projection::yz,Projection::xz,Projection::ECAL_thetaphi
  // ,Projection::HCAL_thetaphi });
  auto display = PFEventDisplay({ViewPane::Projection::xy, ViewPane::Projection::yz});
  std::shared_ptr<GDetector> gdetector(new GDetector(CMSDetector));  // TODO remove shared_ptr?
  display.addToRegister(gdetector, 0);
  display.drawPFEvent(pfEvent);

  // TODO uncomment for commandline
  theApp.Run();

  // sim.testing(); //Write lists of connected items
 

  return EXIT_SUCCESS;
}