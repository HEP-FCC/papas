
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
#include "MergedClusterBuilder.h"
#include "Ruler.h"
#include "Cluster.h"
#include "pTrack.h"
#include "Path.h"
#include "random.h"

#include "AliceDisplay.h"
#include "Id.h"
#include "Log.h"
#include "StringFormatter.h"


#include "datamodel/ParticleCollection.h"
#include "datamodel/EventInfoCollection.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"

// STL
#include <vector>
#include <iostream>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

void processEvent(podio::EventStore& store) {
  
  // read event information
  //const fcc::EventInfoCollection* evinfocoll(nullptr);
  //bool evinfo_available = store.get("EventInfo", evinfocoll);
  /*if(evinfo_available) {
   auto evinfo = evinfocoll->at(0);
   
   if(verbose)
   std::cout << "event number " << evinfo.Number() << std::endl;
   }*/
  
  // read particles
  const fcc::ParticleCollection* ptcs(nullptr);
  bool particles_available = store.get("GenParticle", ptcs);
  if (particles_available){
    std::vector<fcc::Particle> muons;
    // there is probably a smarter way to get a vector from collection?
    bool verbose= true;
    if(verbose)
      std::cout << "particle collection:" << std::endl;
    for(const auto& ptc : *ptcs){
      if(verbose)
        std::cout<<"\t"<<ptc<<std::endl;
      if( ptc.Core().Type == 4 ) {
        muons.push_back(ptc);
      }
    }
  }
}


void dosomerandom ();

//extern int run_tests(int argc, char* argv[]);
using namespace papas;
int main(int argc, char* argv[]) {
  
  auto reader = podio::ROOTReader();
  auto store = podio::EventStore();
  
  // Create CMS detector and simulator
  CMS CMSDetector;
  Simulator sim = Simulator{CMSDetector};
  
  reader.openFile("/Users/alice/fcc/cpp/papas/papas_cc/ee_ZH_Zmumu_Hbb.root");
  store.setReader(&reader);
  const fcc::ParticleCollection* ptcs(nullptr);
  int count=0;
  bool particles_available = store.get("GenParticle", ptcs);
  
  for (const auto& ptc : *ptcs) {
    count+=1;
    
    if (ptc.Core().Status == 1 ) {
      TLorentzVector tlv;
      auto p4 = ptc.Core().P4;
      tlv.SetXYZM(p4.Px, p4.Py, p4.Pz, p4.Mass);
      if (tlv.Pt() > 1e-5  && ( ptc.Core().Type ==22 | ptc.Core().Type > 100) ) {
        PFParticle& pfptc = sim.addParticle(ptc.Core().Type, tlv, TVector3{0, 0, 0});
        PDebug::write("Made {}", pfptc);
        std::cout<< pfptc<<std::endl;
        std::cout << "\t" << ptc << std::endl;
        if (pfptc.pdgId() == 22) {
          
          sim.simulatePhoton(pfptc);
        }
        else if (pfptc.pdgId() == 11) {
          // TODO self.propagate_electron(ptc)
          // smeared_ptc = self.smear_electron(ptc)
        }
        else if (pfptc.pdgId() == 13) {
          // TDOO self.propagate_muon(ptc)
          // smeared_ptc = self.smear_muon(ptc)
        }
        //else if (pfptc.pdgid() == [ 12, 14, 16 ])
        // TODO self.simulate_neutrino(ptc)
        else if (abs(pfptc.pdgId()) >= 100) {  // TODO make sure this is ok
          if (pfptc.charge() && pfptc.pt() < 0.2)
            // to avoid numerical problems in propagation
            continue;
          else
            sim.simulateHadron(pfptc);
        }
      }
    }
  }
  PFEvent pfEvent{sim}; //for python test
  pfEvent.mergeClusters();
  Ids  ids= pfEvent.mergedElementIds();
  PFBlockBuilder bBuilder{pfEvent, ids};
  pfEvent.setBlocks(bBuilder);//for python
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();
  store.clear();
  reader.endOfEvent();
  
  
  
  
  randomgen::setEngineSeed(0xdeadbeef);
  
  
  
  
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
    Simulator siml = Simulator{CMSDetector};
    PFParticle& ptc = siml.addGunParticle(211, -1.5, 1.5, 0.1, 10);
    PDebug::write("Made {}", ptc);
    if (!(ptc.charge() && ptc.pt()<0.2))
      siml.simulateHadron(ptc);
    PFEvent pfEvent{siml}; //for python test
    pfEvent.mergeClusters();
    Ids  ids= pfEvent.mergedElementIds();
    PFBlockBuilder bBuilder{pfEvent, ids};
    pfEvent.setBlocks(bBuilder);//for python
    PFReconstructor pfReconstructor{pfEvent};
    pfReconstructor.reconstruct();
    
  }
  /* for (int i = 0; i < 1000; i++) {
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
  
  
  // setup a PFEvent by copying the simulation tracks and cluster (retaining same identifiers)
  // and using a reference to the history nodes
  //PFEvent pfEvent{sim.smearedEcalClusters(), sim.smearedHcalClusters(), sim.smearedTracks(), sim.historyNodes()};
  //PFEvent pfEvent{sim}; //for python test
  
  
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