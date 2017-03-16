
//
//  example_simple.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/CMS.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/PDebug.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <iostream>
#include "papas/utility/TRandom.h"
#include "papas/utility/Log.h"
#include "papas/utility/TRandom.h"

#include <chrono>


int main(int argc, char* argv[]) {
  
  rootrandom::Random::seed(0xdeadbeef);
  
  if (argc < 2) {
    std::cerr << "Usage: ./example_debug filename [logname]" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  auto pythiaConnector = PythiaConnector(fname);
  
  if (argc == 3) {
    const char* lname = argv[2];
    papas::PDebug::File(lname);  // physics debug output
  }
  papas::Log::init();
  papas::Log::info("Logging Papas ");
  
  // Create CMS detector and PapasManager
  papas::CMS CMSDetector;
  papas::PapasManager papasManager{CMSDetector};
  try {
    // read and process a single event
    unsigned int eventNo = 0;
    pythiaConnector.processEvent(eventNo, papasManager);

    // write out the reconstructed particles to a root file
    pythiaConnector.writeParticlesROOT("simpleeg.root", papasManager.event().particles('r'));

    // write inputs and outputs to screen
    std::cout << "Generated Stable Particles" << std::endl;
    for (const auto& p : papasManager.event().particles('s')) {
      std::cout << "  " << p.second << std::endl;
    }
    std::cout << "Reconstructed Particles" << std::endl;
    for (const auto& p : papasManager.event().particles('r')) {
      std::cout << "  " << p.second << std::endl;
    }

    // testing (move elsewhere)
    pythiaConnector.writeClustersROOT("simpleeg.root", papasManager.event().clusters("em"));

    // produce papas display
    TApplication tApp("theApp", &argc, argv);
    pythiaConnector.displayEvent(papasManager);
    return EXIT_SUCCESS;
  } catch (std::runtime_error& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  } catch (const char* c) {
    std::cerr << c << ". Quitting." << std::endl;
  }
}
    

