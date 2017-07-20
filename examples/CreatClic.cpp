#include "papas/detectors/Clic.h"
#include "papas/detectors/ClicEcal.h"
#include "papas/detectors/ClicHcal.h"
#include "papas/detectors/ClicField.h"
#include "papas/detectors/ClicTracker.h"
#include "papas/detectors/Detector.h"
#include "papas/detectors/Calorimeter.h"

namespace papas {
Clic CreateClic();

Clic CreateClic() {
  double innerEcal = 2.15; double outerEcal= 2.4; double innerHcal = 2.4;double outerHcal= 5.3;
  double depth = 0.25;
  double innerRadius = innerEcal;
  double innerZ = 2.6;
  int nX0 = 23;  //CLIC CDR, page 70, value for CLIC_ILD
  int nLambdaI = 1;  //ibid
/*auto  ecal = std::make_shared<const ClicECAL> (innerRadius=2.15, innerZ=2.6,  depth = .25,  nX0 = 23,  nLambdaI = 1, clusterSize = 0.045, clusterSizePhoton=.015,  clusterSize = 0.045,   etaAcceptance =2.76,  eminBarrel = 0.5,  eminEndcap = 0.5,  eresBarrel = {0.165, 0.010, 0.015},  eResponse = 1. );
                                                 
  */
  
  auto  ecal = std::make_shared<const ClicECAL> (innerRadius=2.15);
                                              

  // HCAL detector element
  auto  hcal =  std::make_shared<const ClicHCAL>();
  // Tracker detector element
  auto tracker = std::make_shared<const ClicTracker>( 2.14, 2.6);
  
  // Field detector element
  auto field = std::make_shared<const Field>(ClicField(2., 3.5, 4.8));
  
  return Clic(ecal,hcal,tracker,field);
                          

  
}

}

