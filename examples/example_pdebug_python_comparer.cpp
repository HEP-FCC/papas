
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/CMS.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/Log.h"
#include "papas/utility/TRandom.h"
// STL
#include <TApplication.h>
#include <chrono>
#include <iostream>

using namespace papas;

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
     PDebug::File(lname);  // physics debug output
  }
  Log::init();
  Log::info("Logging Papas ");

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
      papasManager.clear();
    }

    pythiaConnector.processEvent(i, papasManager);
    /*if (i==0)
      papasManager.display();
    gSystem->ProcessEvents();*/
  }

  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  auto times = std::chrono::duration<double, std::milli>(diff).count();
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
  std::cout << 1000 * nEvents / times << " Evs/s" << std::endl;
  return EXIT_SUCCESS;
}
