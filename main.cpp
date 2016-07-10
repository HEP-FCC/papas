
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "CMS.h"
#include "PFReconstructor.h"
#include "PapasManager.h"
#include "Simulator.h"
#include "PythiaConnector.h"
#include "AliceDisplay.h"
#include "Log.h"

// STL
#include <chrono>
#include <iostream>




using namespace papas;

int example(int argc, char* argv[]);
int example2(int argc, char* argv[]);
int longrun(int argc, char* argv[]);

int main(int argc, char* argv[]) {

  //return example(argc, argv);
  //return example2(argc, argv);
 return longrun(argc, argv);
}

int example(int argc, char* argv[]) {

  if (argc != 2) {
    std::cerr << "Usage: ./mainexe filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  //open the Pythia file fname
  auto pythiaConnector = PythiaConnector(fname);
  
  // Create CMS detector and PapasManager
  CMS CMSDetector;
  papas::PapasManager papasManager{CMSDetector};
  
  //read and process an event
  unsigned int eventNo = 0;
  pythiaConnector.processEvent(eventNo, papasManager);
  
  //write out the reconstructed particles to a root file
  pythiaConnector.writeParticlesROOT("simpleeg.root", papasManager.reconstructedParticles());

  // outputs
  std::cout << "Generated Stable Particles" << std::endl;
  for ( const auto& p : papasManager.rawParticles()) {
    std::cout << "  " << p.second << std::endl;
  }
  std::cout << "Reconstructed Particles" << std::endl;
  for (const auto& p : papasManager.reconstructedParticles()) {
    std::cout << "  " << p.second << std::endl;
  }
  
  papasManager.display();
  return EXIT_SUCCESS;
}

int example2(int argc, char* argv[]) {
  auto pythiaConnector = PythiaConnector("simpleeg.root");
  
  // Create CMS detector and PapasManager
  CMS CMSDetector;
  papas::PapasManager papasManager{CMSDetector};
  
  unsigned int eventNo = 0;
  pythiaConnector.processEvent(eventNo, papasManager);
  
  papasManager.display();
  return EXIT_SUCCESS;
}


int longrun(int argc, char* argv[]) {

  PDebug::On();  // physics debug output
  randomgen::setEngineSeed(0xdeadbeef);
  
 
  
  /*std::cout << randomgen::RandUniform(0, 1).next()<<std::endl;
  std::cout << randomgen::RandUniform(0, 1).next() <<std::endl;
  std::cout << randomgen::RandExponential(3).next()<<std::endl;
  */
  
  if (argc != 2) {
    std::cerr << "Usage: ./mainexe filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  auto pythiaConnector = PythiaConnector(fname);
  
  // Create CMS detector and PapasManager
  CMS CMSDetector;
  papas::PapasManager papasManager{CMSDetector};
  unsigned int eventNo = 0;
  unsigned int nEvents = 10;


  auto start = std::chrono::steady_clock::now();
  
  for (unsigned i = eventNo; i < eventNo + nEvents; ++i) {
    
    PDebug::write("Event: {}", i);
    if (i % 10 == 0) {
      std::cout << "reading event " << i << std::endl;
    }
    if (i == eventNo)
      start = std::chrono::steady_clock::now();
    else {
      /*if (Id::counter() > 900) {
        std::cout << i << "=" << Id::counter() << std::endl;
      }*/
      papasManager.clear();
    }
    pythiaConnector.processEvent(i, papasManager);
  }
  papasManager.display();
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  auto times = std::chrono::duration<double, std::milli>(diff).count();
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
  std::cout << 1000 * nEvents / times << " Evs/s" << std::endl;
  return EXIT_SUCCESS;
}




/*
 //TODO make a gunparticles example out of the following
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



//}  // end namespace papas