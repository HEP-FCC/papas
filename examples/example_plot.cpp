
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

  if (argc < 2) {
    std::cerr << "Usage: ./example_plot filename [logname]" << std::endl;
    return 1;
  }
  if (argc == 3) {
    const char* lname = argv[2];
    PDebug::File(lname);  // physics debug output
  }

  const char* fname = argv[1];
  PythiaConnector pythiaConnector(fname);

  // Create CMS detector and PapasManager
  auto CMSDetector = CreateDefaultCMS();
  papas::PapasManager papasManager{CMSDetector};

  unsigned int eventNo = 0;
  pythiaConnector.processEvent(eventNo, papasManager);
  TApplication tApp("theApp", &argc, argv);
  pythiaConnector.displayEvent(papasManager);
  papasManager.clear();
  return EXIT_SUCCESS;
}
