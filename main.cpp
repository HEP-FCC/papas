
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
//#include "gtest/gtest.h"

#include "CMS.h"
#include "Cluster.h"
#include "MergedClusterBuilder.h"
#include "PFBlockBuilder.h"
#include "PFEvent.h"
#include "PFEventDisplay.h"
#include "SimParticle.h"
#include "PFReconstructor.h"
#include "Path.h"
#include "Ruler.h"
#include "Simulator.h"
#include "displaypfobjects.h"
#include "pTrack.h"
#include "random.h"
#include "PapasManager.h"

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
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include <chrono>

using namespace papas;

Particles makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs);

void processEvent( podio::EventStore& store, PapasManager& papasManager) {
  
  const fcc::ParticleCollection* ptcs(nullptr);
  bool particles_available = store.get("GenParticle", ptcs);
 
  Particles papasparticles= makePapasParticlesFromGeneratedParticles(ptcs);
  papasManager.simulateEvent(std::move(papasparticles));
  papasManager.reconstructEvent();
  store.clear();

  /*
  sort(genStableParticles.begin(), genStableParticles.end(),
       [](const papas::Particle& lhs, const papas::Particle& rhs) { return lhs.e() > rhs.e(); });
 */
  
  
 
}

// extern int run_tests(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  //physics debug output
  PDebug::On();
  
  auto reader = podio::ROOTReader();

  //reader.openFile("/Users/alice/fcc/cpp/papas/papas_cc/ee_ZH_Zmumu_Hbb_50000.root");
  reader.openFile("/afs/cern.ch/user/a/alrobson/Pythia/ee_ZH_Zmumu_Hbb.root");


  unsigned int eventNo = 0;
  unsigned int nEvents = 50000;
  
  bool doDisplay =false;
  if (nEvents==1)
    doDisplay =true;
  
  auto store = podio::EventStore();
  store.setReader(&reader);
  reader.goToEvent(eventNo);
  
  // Create CMS detector and PapasManager
  CMS CMSDetector;
  auto start = std::chrono::steady_clock::now();
  //auto papasManager = PapasManager(CMSDetector) ;

  for(unsigned i=0; i<nEvents; ++i) {
    //papasManager.clear();
    auto papasManager = PapasManager(CMSDetector) ;
     papasManager.clear();
    PDebug::write("Event: {}",eventNo+i);
    
    //if(i%1000==0) {
      std::cout<<"reading event "<<eventNo+i<<std::endl;
    //}

    processEvent(store, papasManager);
    reader.endOfEvent();
    
    if (doDisplay) {
      papasManager.display();
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  auto times=std::chrono::duration <double, std::milli>(diff).count();
  std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
  std::cout << 1000*nEvents/times << " Evs/s" << std::endl;
  return EXIT_SUCCESS;

}
  
  Particles makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs) {
    // turns pythia particles into Papas particles and lodges them in the history
    TLorentzVector tlv;
    Particles particles;
    for (const auto& ptc : *ptcs) {
      if (ptc.Core().Status == 1) {  // only stable ones
        
        auto p4 = ptc.Core().P4;
        tlv.SetXYZM(p4.Px, p4.Py, p4.Pz, p4.Mass);
        // TODO allow more particles through once implemented
        if (tlv.Pt() > 1e-5 && (ptc.Core().Type == 22 | abs(ptc.Core().Type) > 100)) {
          IdType id = Id::makeParticleId();
          particles.emplace(id, papas::Particle(id, abs(ptc.Core().Type), ptc.Core().Charge, tlv, 1));
          PDebug::write("Made Papas{}", particles[id]);
        }
      }
    }
    return std::move(particles);
  }

  /*
  int nParticles=0;
  for (int i = 1; i < nParticles; i++) {
    SimParticle& photon = sim.addParticle(22, 0, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
    PDebug::write("Made {}", photon);
    sim.simulatePhoton(photon);
  }

  //  Hadrons
  nParticles=0;
  for (int i = 1; i < nParticles; i++) {
    SimParticle& hadron = sim.addParticle(211, 1, M_PI / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
    PDebug::write("Made {}", hadron);
    sim.simulateHadron(hadron);
  }

  //particle Gun
  for (int i = 0; i < nParticles; i++) {
    Simulator siml = Simulator{CMSDetector};
    SimParticle& ptc = siml.addGunParticle(211, 1, -1.5, 1.5, 0.1, 10);
    PDebug::write("Made {}", ptc);
    if (!(ptc.charge() && ptc.pt() < 0.2)) siml.simulateHadron(ptc);
    PFEvent pfEvent{siml};  // for python test
    pfEvent.mergeClusters();
    Ids ids = pfEvent.mergedElementIds();
    PFBlockBuilder bBuilder{pfEvent, ids};
    pfEvent.setBlocks(bBuilder);  // for python
    PFReconstructor pfReconstructor{pfEvent};
    pfReconstructor.reconstruct();
  }
   for (int i = 0; i < 1000; i++) {
   SimParticle& ptc = sim.addGunParticle(22, -1.5, 1.5, 0.1, 10);
   PDebug::write("Made {}", ptc);
   if (ptc.charge() && ptc.pt()<0.2)
   continue;
   sim.simulatePhoton(ptc);

   }
   for (int i = 0; i < 1000; i++) {
   SimParticle& ptc = sim.addGunParticle(130, -1.5, 1.5, 0.1, 10);
   PDebug::write("Made {}", ptc);
   if (ptc.charge() && ptc.pt()<0.2)
   continue;
   sim.simulateHadron(ptc);

   }*/




#if 0
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
#endif

  // sim.testing(); //Write lists of connected items



