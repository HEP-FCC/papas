#ifndef Clic_H
#define Clic_H

#include "papas/detectors/Detector.h"
#include "papas/detectors/VolumeCylinder.h"
#include "papas/detectors/ClicField.h"

namespace papas {

/**
 * @file Clic.h
 * @brief Implementation of Clic detector
 */
/// Clic specific implementation of Detector
class Clic : public Detector {
public:
  Clic (std::shared_ptr<const Calorimeter> ecal,
        std::shared_ptr<const Calorimeter> hcal,
        std::shared_ptr<const Tracker> tracker,
        std::shared_ptr<const Field> field,
        double electronAcceptanceMagnitude = 5.,
        double electronAcceptanceEta = 2.5,
        double muonAcceptanceMagnitude = 7.5,
        double muonAcceptanceTheta = 80,
        double electronEnergyFactor = 0.1,
        double muonResolution = 0.02);
  double electronAcceptance(const Track& track) const ;
  double electronEnergyResolution(const Particle& ptc) const ;
  double muonAcceptance(const Track& track) const ;
  double muonResolution(const Particle& ptc) const  ;
private:
  double m_electronAcceptanceMagnitude;
  double m_electronAcceptanceEta;
  double m_muonAcceptanceMagnitude;
  double m_muonAcceptanceTheta;
  double m_electronEnergyFactor;
  double m_muonResolution;
  
};

}  // end namespace papas
#endif
