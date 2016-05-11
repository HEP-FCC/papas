
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>

#include "TApplication.h"
#include "TROOT.h"
#include "gtest/gtest.h"

#include "CMS.h"
#include "PFEventDisplay.h"
#include "Simulator.h"
#include "displaygeometry.h"
//#include "displaypfobjects.h"
#include "Cluster.h"
#include "PFBlock.h"
#include "PFBlockBuilder.h"
#include "PFEvent.h"
#include "PFParticle.h"
#include "PFReconstructor.h"
#include "Track.h"
#include "path.h"

#include "MyAnalysis.h"

extern int run_tests(int argc, char* argv[]);

int main(int argc, char* argv[]) {

  // Create CMS detector and simulator
  CMS CMSDetector;
  Simulator sim = Simulator{CMSDetector};

  // Make Some Photons
  for (int i = 1; i < 1; i++) {
    PFParticle& photon = sim.addParticle(22, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
    sim.simulatePhoton(photon);
  }

  // Make Some Hadrons
  for (int i = 0; i < 1; i++) {
    PFParticle& hadron = sim.addParticle(211, M_PI / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
    sim.simulateHadron(hadron);
  }

  // setup a PFEvent by copying the simulation tracks and cluster (retaining same identifiers)
  // and using a reference to the history nodes
  PFEvent pfEvent{sim.smearedEcalClusters(), sim.smearedHcalClusters(), sim.smearedTracks(), sim.historyNodes()};

  // Reconstruct
  PFBlockBuilder bBuilder{pfEvent};
  pfEvent.setBlocks(std::move(bBuilder.blocks()));
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();

  // Now move on to Displaying results

  // ROOT App to allow graphs to be plotted
  TApplication theApp("App", &argc, argv);
  if (gROOT->IsBatch()) {
    fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
    return 1;
  }

  // All displays
  // Display display = Display({Projection::xy,Projection::yz,Projection::xz,Projection::ECAL_thetaphi
  // ,Projection::HCAL_thetaphi });
  auto display = PFEventDisplay({Projection::xy, Projection::yz});
  std::shared_ptr<GDetector> gdetector(new GDetector(CMSDetector));  // TODO remove shared_ptr?
  display.addToRegister(gdetector, 0);
  display.drawPFEvent(pfEvent);

  // TODO uncomment for commandline
  // run theApp.Run();

  // sim.testing(); //Write lists of connected items
  // int test = run_tests(argc, argv);

  return EXIT_SUCCESS;
}