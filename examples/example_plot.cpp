// Produces an event display via ROOT (includes writing jpg files)
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/cms/CMS.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/Logger.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/TRandom.h"

#include <TApplication.h>

using namespace papas;

int main(int argc, char* argv[]) {

  rootrandom::Random::seed(0xdeadbeef);
  papaslog::papaslogger = papaslog::getDefaultLogger("Papas LOG", papaslog::Logging::ERROR);

  if (argc != 2) {
    std::cerr << "Usage: ./example_plot filename " << std::endl;
    return 1;
  }

  const char* fname = argv[1];
  PythiaConnector pythiaConnector(fname);

  // Create CMS detector and PapasManager
  auto CMSDetector = CreateDefaultCMS();
  papas::PapasManager papasManager{CMSDetector};

  unsigned int eventNo = 0;
  pythiaConnector.processEvent(eventNo, papasManager);
  TApplication tApp("theApp", &argc, argv);
  //display to screen and to jpeg
  pythiaConnector.displayEvent(papasManager);
  papasManager.clear();
  tApp.Run();
  return EXIT_SUCCESS;
}
