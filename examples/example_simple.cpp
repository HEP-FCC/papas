
//Example to run papas and create output root file and plots (to screen and to jpeg)
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/cms/CMS.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/Logger.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/TRandom.h"
#include <TApplication.h>
#include <TCanvas.h>
#include <iostream>

#include <chrono>

int main(int argc, char* argv[]) {

  rootrandom::Random::seed(0xdeadbeef);
  papaslog::papaslogger = papaslog::getDefaultLogger("Papas LOG", papaslog::Logging::ERROR);

  if (argc != 2) {
    std::cerr << "Usage: ./example_debug filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  PythiaConnector pythiaConnector(fname);

  // Create CMS detector and PapasManager
  papas::CMS CMSDetector = papas::CreateDefaultCMS();
  papas::PapasManager papasManager{CMSDetector};
  try {
    // read and process a single event
    unsigned int eventNo = 0;
    pythiaConnector.processEvent(eventNo, papasManager);

    // write out the reconstructed particles to a root file
    pythiaConnector.writeParticlesROOT("simpleeg.root", papasManager.event().particles('r'));

    // write inputs and outputs to screen
    std::cout << "Generated Stable Particles: " << papasManager.event().particles('s').size() << std::endl;
    for (const auto& p : papasManager.event().particles('s')) {
      std::cout << "  " << p.second << std::endl;
    }
    std::cout << "Reconstructed Particles: " << std::endl << papasManager.event().particles('r').size() << std::endl;
    for (const auto& p : papasManager.event().particles('r')) {
      std::cout << "  " << p.second << std::endl;
    }
    TApplication tApp("theApp", &argc, argv);
    //display to screen and to jpeg
    pythiaConnector.displayEvent(papasManager);
    papasManager.clear();
    tApp.Run();

    return EXIT_SUCCESS;
  } catch (std::runtime_error& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  } catch (const char* c) {
    std::cerr << c << ". Quitting." << std::endl;
  }
}
