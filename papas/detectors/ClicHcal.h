#ifndef ClicHCAL_H
#define ClicHCAL_H

#include "papas/detectors/Calorimeter.h"

#include <vector>

namespace papas {

class Particle;
class Cluster;
class Material;
class VolumeCylinder;

/// Clic specific implementation of  HCAL Detector element
///
class ClicHCAL : public Calorimeter {
public:
  /** Constructor
   *
   * @param[in] volume The HCAL cyclinders
   * @param[in] material HCAL material
   * @param[in] eta_crack ask Colin
   * @param[in] emin vector of minimum energy { barrel, endcap}
   * @param[in] eres vector of  vectors of energy resolution { barrel, endcap}
   */
  ClicHCAL(double innerRadius = 2.4,
           double innerZ = 2.85,
           double outerRadius = 4.8,
           double outerZ = 5.3,
           double clusterSize = 0.25,
           std::vector<double> eresBarrel = {0.6, 0., 0.025},
           double x0 = 0.018,
           double lambdaI = 0.17,
           double eResponse = 1,
           double etaAcceptance = 2.76);

  double clusterSize(const Particle& ptc) const override;
  bool acceptance(const Cluster& cluster) const override;
  double energyResolution(double energy, double eta = 0) const override;
  double energyResponse(double energy, double eta = 0) const override;
  // TODOAJR space_resolution(self, ptc):
private:
  double m_clusterSize;
  double m_etaAcceptance;
  std::vector<double> m_eresBarrel;
  double m_eResponse;  ///< energy response default to 1.
};

}  // end namespace papas
#endif
