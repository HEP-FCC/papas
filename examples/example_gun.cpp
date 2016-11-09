
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "papas/utility/PDebug.h"
#include "TApplication.h"
#include "TROOT.h"
//#include "gtest/gtest.h"

#include "papas/detectors/CMS.h"
#include "papas/datatypes/Cluster.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFEvent.h"
#include "papas/reconstruction/PFEventDisplay.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/datatypes/Path.h"
#include "papas/graphtools/Ruler.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/simulation/Simulator.h"
#include "papas/datatypes/Track.h"
#include "papas/utility/Random.h"

#include "papas/display/AliceDisplay.h"
#include "papas/datatypes/Id.h"
#include "papas/utility/Log.h"
#include "papas/utility/StringFormatter.h"

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

int Gun() {
  PDebug::On("/Users/alice/work/Outputs/gun_cpp_output.txt");  // physics debug output
  int nParticles = 10;
  papas::CMS CMSDetector;
  Nodes history;
  for (int i = 0; i < nParticles; i++) {
    auto siml = Simulator(CMSDetector, history);
    SimParticle& ptc = siml.addGunParticle(211, 1, -1.5, 1.5, 0.1, 10);
    PDebug::write("Made {}", ptc);
    // simulate Hadron
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
