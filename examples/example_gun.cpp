
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
#include "PDebug.h"
//#include "gtest/gtest.h"

#include "CMS.h"
#include "Cluster.h"
#include "MergedClusterBuilder.h"
#include "PFBlockBuilder.h"
#include "PFEvent.h"
#include "PFEventDisplay.h"
#include "PFReconstructor.h"
#include "PapasManager.h"
#include "Path.h"
#include "Ruler.h"
#include "SimParticle.h"
#include "Simulator.h"
#include "pTrack.h"
#include "random.h"

#include "AliceDisplay.h"
#include "Id.h"
#include "Log.h"
#include "StringFormatter.h"

#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"

// STL
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

using namespace papas;

Particles makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs);
void processEvent(podio::EventStore& store, PapasManager& papasManager);
int Gun();

int main(int argc, char* argv[]) {
  randomgen::setEngineSeed(0xdeadbeef);
  return Gun();
  // return longrun(argc,argv);
}


int Gun()
{
  PDebug::On("/Users/alice/work/Outputs/gun_cpp_output.txt");  // physics debug output
  int nParticles=10;
  papas::CMS CMSDetector;
  Nodes history;
  for (int i = 0; i < nParticles; i++) {
    auto siml = Simulator(CMSDetector, history);
    SimParticle& ptc = siml.addGunParticle(211, 1, -1.5, 1.5, 0.1, 10);
    PDebug::write("Made {}", ptc);
    //simulate Hadron
    if (!(ptc.charge() && ptc.pt() < 0.2)) siml.simulateParticle(ptc);
    PFEvent pfEvent{siml};  // for python test
    pfEvent.mergeClusters();
    Ids ids = pfEvent.mergedElementIds();
    PFBlockBuilder bBuilder{ids, pfEvent};
    PFReconstructor pfReconstructor{pfEvent};
    pfReconstructor.reconstruct(bBuilder.blocks());
  }
  return EXIT_SUCCESS;
}
