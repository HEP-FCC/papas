
#include <iostream>
#include <stdio.h>

#include "PythiaConnector.h"
#include "papas/detectors/clic/Clic.h"
#include "papas/detectors/cms/CMS.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/utility/Log.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/TRandom.h"
// STL
#include <TApplication.h>
#include <chrono>
#include <iostream>

#include "papas/detectors/clic/ClicEcal.h"
#include "papas/detectors/clic/ClicField.h"
#include "papas/detectors/clic/ClicHcal.h"
#include "papas/detectors/clic/ClicTracker.h"

#include "papas/detectors/cms/CMSEcal.h"
#include "papas/detectors/cms/CMSField.h"
#include "papas/detectors/cms/CMSHcal.h"
#include "papas/detectors/cms/CMSTracker.h"

using namespace papas;

Clic CreateClic() {

  std::vector<double> emin{0.5, 0.5};
  std::vector<double> eresBarrel{0.165, 0.010, 0.015};
  auto ecal = std::make_shared<const ClicECAL>(2.15,   // innerRadius
                                               2.6,    //  innerZ
                                               .25,    //  depth
                                               .015,   // clusterSizePhoton
                                               0.045,  // clusterSize
                                               2.76,   // etaAcceptance
                                               emin,
                                               eresBarrel,
                                               23,  // nX0
                                               1,   // nLambdaI
                                               1.   // eResponse
                                               );

  std::vector<double> eresBarrelHCAL{0.6, 0., 0.025};
  auto hcal = std::make_shared<const ClicHCAL>(2.4,             // innerRadius
                                               2.85,            // innerZ
                                               4.8,             // outerRadius
                                               5.3,             // outerZ
                                               0.25,            // clusterSize
                                               eresBarrelHCAL,  // eresBarrel
                                               0.018,           // x0
                                               0.17,            // lambdaI
                                               1.,              // eResponse
                                               2.76);           // etaAcceptance

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
                                                 4.8,   // outerZ
                                                 0.0,   // material x0
                                                 0.0);  // material lambdaI

  return Clic(ecal, hcal, tracker, field);
}

CMS CreateCMS() {

  std::vector<std::vector<double>> eresECAL = {{4.22163e-02, 1.55903e-01, 7.14166e-03},
                                               {-2.08048e-01, 3.25097e-01, 7.34244e-03}};
  std::vector<std::vector<double>> erespECAL = {{1.00071, -9.04973, -2.48554}, {9.95665e-01, -3.31774, -2.11123}};
  std::vector<double> emin{0.3, 1};
  auto ecal = std::make_shared<const CMSECAL>(1.3,     // innerRadius
                                              2,       // innerZ
                                              1.55,    // outerRadius
                                              2.1,     // outerZ
                                              8.9e-3,  // x0
                                              0.275,   // lambdaI
                                              0.04,    // clusterSizePhoton
                                              0.07,    // clusterSize
                                              1.479,   // etaCrack
                                              2.93,    // etaAcceptanceThreshold
                                              0.2,     // ptAcceptanceThreshold
                                              1.479,   // etaEndcapMin
                                              3.,      // etaEndcapMax
                                              emin,
                                              eresECAL,
                                              erespECAL);

  std::vector<std::vector<double>> eresHCAL = {{0.8062, 2.753, 0.1501}, {6.803e-06, 6.676, 0.1716}};
  std::vector<std::vector<double>> erespHCAL = {{1.036, 4.452, -2.458}, {1.071, 9.471, -2.823}};
  std::vector<double> acceptanceParameters = {1.,  // energy
                                              1.,           -1.9381,     -1.75330,    3.,       1.1,      10., 1.05634,
                                              -1.66943e-01, 1.05997e-02, 8.09522e-01, -9.90855, -5.30366, 5.,  7.};

  auto hcal = std::make_shared<const CMSHCAL>(1.9,   // innerRadius
                                              2.6,   // innerZ
                                              2.9,   // outerRadius
                                              3.6,   // outerZ
                                              0.2,   // clusterSize
                                              0.,    // x0
                                              0.17,  // lambdaI
                                              1.3,   // etaCrack
                                              eresHCAL,
                                              erespHCAL,
                                              acceptanceParameters);

  auto tracker = std::make_shared<const CMSTracker>(1.29,    // outerRadius= 1.29
                                                    1.99,    // outerZ  = 1.99
                                                    0,       // x0 = 0,
                                                    0,       // lambdaI = 0,
                                                    1.1e-2,  // resolution = 1.1e-2
                                                    0.5,     //  ptThreshold = 0.5
                                                    1.35,    // double etaThresholdLow = 1.35,
                                                    0.95,    // double ptProbabilityLow = 0.95,
                                                    2.5,     // double etaThresholdHigh = 2.5,
                                                    0.9);    // double ptProbabilityHigh = 0.9);

  auto field = std::make_shared<const CMSField>(3.8,   // field magnitude
                                                2.9,   // outerRadius
                                                3.6,   // outerZ
                                                0.0,   // material x0
                                                0.0);  // material lambdaI
  return CMS(ecal, hcal, tracker, field);
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

  auto CMSDetector = CreateCMS();
  auto papasManager = papas::PapasManager(CMSDetector);

  // auto clic = CreateClic();
  // auto papasManager = papas::PapasManager(clic);

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
