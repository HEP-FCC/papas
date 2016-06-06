
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
#include "pTrack.h"
#include "Path.h"
#include "random.h"

#include "AliceDisplay.h"
#include "Id.h"
#include "Log.h"
#include "StringFormatter.h"
void dosomerandom ();

//extern int run_tests(int argc, char* argv[]);
using namespace papas;
int main(int argc, char* argv[]) {
  
  // Create CMS detector and simulator
  CMS CMSDetector;
  Simulator sim = Simulator{CMSDetector};

  // Make Some Photons
  for (int i = 1; i < 0; i++) {
    PFParticle& photon = sim.addParticle(22, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
    PDebug::write("Made {}", photon);
    sim.simulatePhoton(photon);
  }

  // Make Some Hadrons
  for (int i = 1; i < 0; i++) {
    PFParticle& hadron = sim.addParticle(211, M_PI / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
    PDebug::write("Made {}", hadron);
    sim.simulateHadron(hadron);
  }
  
  //Python comparison step 0.1
  //PFParticle& hadron = sim.addParticle(211, 0.9, -0.19, 47.2);
  for (int i = 0; i < 0 /*1000*/; i++) {
   PFParticle& ptc = sim.addGunParticle(211, -1.5, 1.5, 0.1, 10);
    PDebug::write("Made {}", ptc);
    if (ptc.charge() && ptc.pt()<0.2)
      continue;
    sim.simulateHadron(ptc);
    
  }
  for (int i = 0; i < 1; i++) {
    PFParticle& ptc = sim.addGunParticle(22, -1.5, 1.5, 0.1, 10);
    PDebug::write("Made {}", ptc);
    if (ptc.charge() && ptc.pt()<0.2)
      continue;
    sim.simulatePhoton(ptc);
    
  }


  // setup a PFEvent by copying the simulation tracks and cluster (retaining same identifiers)
  // and using a reference to the history nodes
  //PFEvent pfEvent{sim.smearedEcalClusters(), sim.smearedHcalClusters(), sim.smearedTracks(), sim.historyNodes()};
  PFEvent pfEvent{sim}; //for python test
  

  //Log::info() << pfEvent;
  //PDebug::write() << pfEvent;
  
  //PDebug::write( pfEvent);
  //PDebug::write( "things {:8s}{:1d}{}", "happen", 5, pfEvent);
  //Log::log()->flush();
  PDebug::log()->flush();



  // Reconstruct
  PFBlockBuilder bBuilder{pfEvent};
  //pfEvent.setBlocks(std::move(bBuilder.blocks()));
  pfEvent.setBlocks(bBuilder);//for python
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();

  // Now move on to Displaying results
  Log::info() << pfEvent;
  Log::log()->flush();

  PFApp myApp{};
  myApp.display(pfEvent, CMSDetector);
  
  myApp.run();
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


void dosomerandom () {
  randomgen::RandUniform runi1{0,1};
  randomgen::RandUniform runi2{0, 1};
  randomgen::RandExponential rexp1{3};
  randomgen::RandExponential rexp2{4};
  
  runi1.setSeed(0xdeadbeef);
  
  
  std::cout << runi1.next()<< std::endl;
  std::cout << runi2.next()<< std::endl;
  std::cout << rexp1.next()<< std::endl;
  std::cout << rexp2.next()<< std::endl;
  
  runi1.setSeed(0xdeadbeef);
  std::cout << runi1.next()<< std::endl;
  std::cout << runi2.next()<< std::endl;
  std::cout << rexp1.next()<< std::endl;
  std::cout << rexp2.next()<< std::endl;
  
  runi1.setSeed(0xdeadbeef);
  for (int i =0; i<6 ; i++)
    std::cout << runi2.next()<< std::endl;
  
  
  randomgen::RandExponential rexp{49.3};
  rexp.setSeed(100);
  std::cout << rexp.next() << rexp.next()<<std::endl;;
  
  
}