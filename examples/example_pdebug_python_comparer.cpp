//Example to produce physics debug output
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/clic/Clic.h"
#include "papas/detectors/cms/CMS.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/Logger.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/TRandom.h"
// STL
#include <TApplication.h>
#include <chrono>
#include <iostream>
#include <memory>

using namespace papas;

int main(int argc, char* argv[]) {

  rootrandom::Random::seed(0xdeadbeef);
  //start the logger
  papaslog::papaslogger = papaslog::getDefaultLogger("Papas LOG", papaslog::Logging::VERBOSE);

  if (argc != 4) {
    std::cerr << "Usage: ./example_debug filename detector pdebug_log_name" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  PythiaConnector pythiaConnector(fname);

  std::string detname = argv[2];

  const char* lname = argv[3];
  PDebug::File(lname);  // physics debug output
 
  PAPASLOG_VERBOSE("Starting run");
  auto cmsdetector = CreateDefaultCMS();
  auto clicdetector = CreateDefaultClic();

  std::shared_ptr<papas::PapasManager> papasManager;
  if (detname == "CMS") {
    papasManager = std::make_shared<papas::PapasManager>(cmsdetector);
  } else if (detname == "CLIC")
    papasManager = std::make_shared<papas::PapasManager>(clicdetector);
  else
    PAPASLOG_ERROR( "Detector not recognised should be CMS or CLIC ");

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
      papasManager->clear();
    }

    pythiaConnector.processEvent(i, papasManager);
  }

  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  auto times = std::chrono::duration<double, std::milli>(diff).count();
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
  std::cout << 1000 * nEvents / times << " Evs/s" << std::endl;
  return EXIT_SUCCESS;
}
