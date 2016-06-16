
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
#include "Cluster.h"
#include "MergedClusterBuilder.h"
#include "PFBlockBuilder.h"
#include "PFEvent.h"
#include "PFEventDisplay.h"
#include "PFParticle.h"
#include "PFReconstructor.h"
#include "Path.h"
#include "Ruler.h"
#include "Simulator.h"
#include "displaypfobjects.h"
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
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

using namespace papas;

void processEvent(podio::EventStore& store, const fcc::ParticleCollection* ptcs, Simulator& sim) {

  //temp for mem leak const fcc::ParticleCollection* ptcs(nullptr);
  int count = 0;
  //temp for mem leak bool particles_available = store.get("GenParticle", ptcs);
  
  std::vector<papas::Particle> particles;
  
  for (const auto& ptc : *ptcs) {
    count += 1;
    if (ptc.Core().Status == 1) {
      TLorentzVector tlv;
      auto p4 = ptc.Core().P4;
      tlv.SetXYZM(p4.Px, p4.Py, p4.Pz, p4.Mass);
      if (tlv.Pt() > 1e-5 && (ptc.Core().Type == 22 | abs(ptc.Core().Type) > 100)) {
        particles.push_back(papas::Particle(-1, abs(ptc.Core().Type), ptc.Core().Charge, tlv, 1));
      }
    }
  }
  
  sort(particles.begin(), particles.end(),
       [](const papas::Particle& lhs, const papas::Particle& rhs) { return lhs.e() > rhs.e(); });
  count = 0;
  for (const auto& ptc : particles) {
    count += 1;
    
    PFParticle& pfptc = sim.addParticle(ptc.pdgId(), ptc.charge(),ptc.p4(), TVector3{0, 0, 0});
    PDebug::write("Made {}", pfptc);
    //std::cout << "\t" << pfptc << std::endl;
    if (pfptc.pdgId() == 22) {
      
      sim.simulatePhoton(pfptc);
    } else if (pfptc.pdgId() == 11) {
      // TODO self.propagate_electron(ptc)
      // smeared_ptc = self.smear_electron(ptc)
    } else if (pfptc.pdgId() == 13) {
      // TDOO self.propagate_muon(ptc)
      // smeared_ptc = self.smear_muon(ptc)
    }
    // else if (pfptc.pdgid() == [ 12, 14, 16 ])
    // TODO self.simulate_neutrino(ptc)
    else if (abs(pfptc.pdgId()) >= 100) {  // TODO make sure this is ok
      if (pfptc.charge() && pfptc.pt() < 0.2)
        // to avoid numerical problems in propagation
        continue;
      else
        sim.simulateHadron(pfptc);
    }
  }
  // setup a PFEvent by copying the simulation tracks and cluster (retaining same identifiers)
  // and using a reference to the history nodes
  PFEvent pfEvent{sim.smearedEcalClusters(), sim.smearedHcalClusters(), sim.smearedTracks(), sim.historyNodes()};
  //PFEvent pfEvent{sim};  // for python test
  pfEvent.mergeClusters();
  Ids ids = pfEvent.mergedElementIds();
  PFBlockBuilder bBuilder{pfEvent, ids};
  pfEvent.setBlocks(bBuilder);  // for python
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();
  pfEvent.setReconstructedParticles(pfReconstructor.particles());
  store.clear();
}

// extern int run_tests(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  
  auto reader = podio::ROOTReader();
  //reader.openFile("/Users/alice/fcc/cpp/papas/papas_cc/ee_ZH_Zmumu_Hbb_50000.root");
  reader.openFile("/Users/alice/fcc/cpp/papas/papas_cc/ee_ZH_Zmumu_Hbb.root");

  unsigned int eventNo = 0;
  unsigned int  nEvents=1;
  
  if (argc ==2) {
    eventNo=(atoi)(argv[1]);
  }
  if (argc ==3) {  //defaults to running 1 event
    nEvents=(atoi)(argv[2]);
  }

    // Create CMS detector and simulator
  CMS CMSDetector;
  Simulator sim = Simulator{CMSDetector};
  auto store = podio::EventStore();
  bool verbose=true;
  store.setReader(&reader);
  reader.goToEvent(eventNo);
 
  const fcc::ParticleCollection* ptcs(nullptr);
  int count = 0;
  bool particles_available = store.get("GenParticle", ptcs);
  
  
  for(unsigned i=0; i<1; ++i) {
    PDebug::write("Event: {}",eventNo+i);
    Simulator siml = Simulator{CMSDetector};
    if(i%1000==0) {
      std::cout<<"reading event "<<eventNo+i<<std::endl;
    }
    if(i>10) {
      verbose = false;
    }
    processEvent(store, ptcs, siml);
    //temp for mem leak store.clear();
    //temp for mem leak reader.endOfEvent();
    
    
    //bool doDisplay =false;
    bool doDisplay =true;
    if (doDisplay) {
      PFEvent pfEvent{siml};
      Log::log()->flush();
      PFApp myApp{};
      myApp.display(pfEvent, CMSDetector);
      myApp.run();
      std::cout <<"plotted";
    }

  }

  /*
  int nParticles=0;
  for (int i = 1; i < nParticles; i++) {
    PFParticle& photon = sim.addParticle(22, 0, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
    PDebug::write("Made {}", photon);
    sim.simulatePhoton(photon);
  }

  //  Hadrons
  nParticles=0;
  for (int i = 1; i < nParticles; i++) {
    PFParticle& hadron = sim.addParticle(211, 1, M_PI / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
    PDebug::write("Made {}", hadron);
    sim.simulateHadron(hadron);
  }

  //particle Gun
  for (int i = 0; i < nParticles; i++) {
    Simulator siml = Simulator{CMSDetector};
    PFParticle& ptc = siml.addGunParticle(211, 1, -1.5, 1.5, 0.1, 10);
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
   PFParticle& ptc = sim.addGunParticle(22, -1.5, 1.5, 0.1, 10);
   PDebug::write("Made {}", ptc);
   if (ptc.charge() && ptc.pt()<0.2)
   continue;
   sim.simulatePhoton(ptc);

   }
   for (int i = 0; i < 1000; i++) {
   PFParticle& ptc = sim.addGunParticle(130, -1.5, 1.5, 0.1, 10);
   PDebug::write("Made {}", ptc);
   if (ptc.charge() && ptc.pt()<0.2)
   continue;
   sim.simulateHadron(ptc);

   }*/


    return 0;

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

  return EXIT_SUCCESS;
}

