//
//  example_loop.cpp
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
#include "papas/utility/TRandom.h"

// STL
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
  papas:: PDebug::File("physics.txt");
  // randomgen::setEngineSeed(0xdeadbeef);  // make results reproduceable
  rootrandom::Random::seed(0xdeadbeef);

  if (argc != 2) {
    std::cerr << "Usage: ./mainexe filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  // open the Pythia file fname
  try {
    auto pythiaConnector = PythiaConnector(fname);
#if WITHSORT
    std::cout << "doing sorting";
#else
    std::cout << "no sort";
#endif
    // Create CMS detector and PapasManager
    papas::CMS CMSDetector;
    auto papasManager = papas::PapasManager(CMSDetector);

    unsigned int eventNo = 0;
    unsigned int nEvents = 100;

    auto start = std::chrono::steady_clock::now();

    for (unsigned i = eventNo; i < eventNo + nEvents; ++i) {

      papas::PDebug::write("Event: {}", i);
      if (i % 10 == 0) {
        std::cout << "reading event " << i << std::endl;
      }
      if (i == eventNo)
        start = std::chrono::steady_clock::now();
      else {
        papasManager.clear();
      }
      pythiaConnector.processEvent(i, papasManager);
    }

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    auto times = std::chrono::duration<double, std::milli>(diff).count();
    std::cout << times << " ms" << std::endl;
    std::cout << 1000 * nEvents / times << " Evs/s" << std::endl;
    return EXIT_SUCCESS;
  } catch (std::runtime_error& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  }
}
