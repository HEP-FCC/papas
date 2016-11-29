
//
//  example_simple.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "papas/detectors/CMS.h"
#include "papas/utility/PDebug.h"
#include "papas/reconstruction/PapasManager.h"
#include "PythiaConnector.h"

#include <iostream>

#include <iostream>

using namespace std;

#include <TApplication.h>
#include <TCanvas.h>

int main(int argc, char* argv[]) {

  randomgen::setEngineSeed(0xdeadbeef);  // make results reproduceable

  if (argc != 2) {
    std::cerr << "Usage: ./mainexe filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  // open the Pythia file fname
  try {
    auto pythiaConnector = PythiaConnector(fname);

    // Create CMS detector and PapasManager
    papas::CMS CMSDetector;
    auto papasManager = papas::PapasManager(CMSDetector);

    // read and process a single event
    unsigned int eventNo = 0;
    pythiaConnector.processEvent(eventNo, papasManager);

    // write out the reconstructed particles to a root file
    pythiaConnector.writeParticlesROOT("simpleeg.root", papasManager.reconstructedParticles());
    pythiaConnector.writeClustersROOT("simpleeg.root", papasManager.pfEvent().ecalMergedClusters());

    // write inputs and outputs to screen
    std::cout << "Generated Stable Particles" << std::endl;
    for (const auto& p : papasManager.rawParticles()) {
      std::cout << "  " << p.second << std::endl;
    }
    std::cout << "Reconstructed Particles" << std::endl;
    for (const auto& p : papasManager.reconstructedParticles()) {
      std::cout << "  " << p.second << std::endl;
    }
    // produce papas display
    TApplication tApp("theApp", &argc, argv);
    papasManager.display(false);
    papasManager.show();

    return EXIT_SUCCESS;
  } catch (std::runtime_error& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  } catch (const char* c) {
    std::cerr << c << ". Quitting." << std::endl;
  }
}
