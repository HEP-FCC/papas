#ifndef CMS_H
#define CMS_H

#include "papas/detectors/Detector.h"
#include "papas/detectors/VolumeCylinder.h"
#include "papas/detectors/CMSField.h"

namespace papas {
  
  class CMSField;

/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */
/// CMS specific implementation of Detector
class CMS : public Detector {
public:

  //CMS(double innerEcal = 1.30, double outerEcal= 1.55, double innerHcal = 1.9,double outerHcal= 2.9);
  CMS(double innerEcal = 1.30, double outerEcal= 1.55, double innerHcal = 1.9,double outerHcal= 2.9, std::shared_ptr<const Field> field = std::make_shared<const Field>(CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8)));
  double electronAcceptance(const Track& track) const override ;
  double electronEnergyResolution(const Particle& ptc) const  override;
  double muonAcceptance(const Track& track) const override;
  double muonResolution(const Particle& ptc) const override ;
private:
};

}  // end namespace papas
#endif
