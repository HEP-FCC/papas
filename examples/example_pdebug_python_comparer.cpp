
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/CMS.h"
#include "papas/detectors/Clic.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/Log.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/TRandom.h"
// STL
#include <TApplication.h>
#include <chrono>
#include <iostream>

#include "papas/detectors/ClicEcal.h"
#include "papas/detectors/ClicField.h"
#include "papas/detectors/ClicHcal.h"
#include "papas/detectors/ClicTracker.h"

using namespace papas;

Clic CreateClic() {

  auto ecal = std::make_shared<const ClicECAL>();

  std::vector<double> eresBarrel{0.6, 0., 0.025};
  auto hcal = std::make_shared<const ClicHCAL>(2.4,         // innerRadius
                                               2.85,        // innerZ
                                               4.8,         // outerRadius
                                               5.3,         // outerZ
                                               0.25,        // clusterSize
                                               eresBarrel,  // eresBarrel
                                               0.018,       // x0
                                               0.17,        // lambdaI
                                               1.,          // eResponse
                                               2.76);       // etaAcceptance

  std::map<int, std::pair<double, double>> resMap = {{90, {8.2e-2, 9.1e-2}},
                                                     {80, {8.2e-4, 9.1e-3}},
                                                     {30, {9.9e-5, 3.8e-3}},
                                                     {20, {3.9e-5, 1.6e-3}},
                                                     {10, {2e-5, 7.2e-4}}};
  auto tracker = std::make_shared<const ClicTracker>(2.14,    // outerRadius
                                                     2.6,     // outerZ
                                                     0,       // x0 = 0,
                                                     0,       // lambdaI = 0,
                                                     0.8,     // thetaParam = 0.8,
                                                     resMap,  // resmap
                                                     0.4,     // double ptThresholdLow = 0.4,
                                                     0.95,    // double ptProbabilityLow = 0.95,
                                                     2.,      // double ptThresholdHigh = 2.,
                                                     0.99);   // double ptProbabilityHigh = 0.99);

  auto field = std::make_shared<const ClicField>(2,     // field magnitude
                                                 3.5,   // outerRadius
                                                 4.8);  // outerZ

  return Clic(ecal, hcal, tracker, field);
}

int main(int argc, char* argv[]) {

  rootrandom::Random::seed(0xdeadbeef);

  if (argc < 2) {
    std::cerr << "Usage: ./example_debug filename [logname]" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  PythiaConnector pythiaConnector(fname);

  if (argc == 3) {
    const char* lname = argv[2];
    PDebug::File(lname);  // physics debug output
  }
  Log::init();
  Log::info("Logging Papas ");

  // Create CMS detector and PapasManager
  auto field = std::make_shared<const CMSField>(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8);
  auto CMSDetector = CMS(1.30, 1.55, 1.9, 2.9, field);
  // auto papasManager = papas::PapasManager(CMSDetector);
  auto clic = CreateClic();

  // Clic ClicDetector;
  auto papasManager = papas::PapasManager(clic);
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
