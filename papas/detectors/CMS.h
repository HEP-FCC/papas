#ifndef CMS_H
#define CMS_H

#include "papas/detectors/Detector.h"

namespace papas {

/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */
/// CMS specific implementation of Detector
class CMS : public Detector {
public:

  CMS(double innerEcal = 1.30, double outerEcal= 1.55, double innerHcal = 1.9,double outerHcal= 2.9);
private:
};

}  // end namespace papas
#endif
