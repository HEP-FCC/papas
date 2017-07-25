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
   @param[in] innerRadius radius of inner cylinder of HCAL
   @param[in] innerZ z of inside of HCAL
   @param[in] outerRadius radius of outer cylinder of HCAL
   @param[in] outerZ z of outside of HCAL
   @param[in] X0 of HCAL material
   @param[in] lambdaI lambdaI of HCAL material
   @param[in] clusterSize size of HCAL clusters
   @param[in] eresBarrel energy resolution parameters for barrel, length 3, defaults to {0.6, 0., 0.025}
   @param[in] eResponse energy response
   @param[in] etaAcceptance max eta for acceptance
   @param[in] energyAcceptance min energy for acceptance
   */
  ClicHCAL(double innerRadius = 2.4,
           double innerZ = 2.85,
           double outerRadius = 4.8,
           double outerZ = 5.3,
           double clusterSize = 0.25,
           std::vector<double> eresBarrel = {0.6, 0., 0.025},
           double x0 = 0.018,
           double lambdaI = 0.17,
           double eResponse = 1.,
           double etaAcceptance = 2.76,
           double energyAcceptance = 1.);

  /**Size of a cluster deposit in the HCAL
   @param[in] particle particle
   */
  double clusterSize(const Particle& ptc) const override;

  /** Decides whether a cluster will be seen by a detector
   @param[in]  cluster the cluster to be analysed
   @return true if cluster is detected, false it if is too small to be seen
   */
  bool acceptance(const Cluster& cluster) const override;

  /** energy resolution for the HCAL
   @param[in] energy energy
   @param[in] eta eta   */
  double energyResolution(double energy, double eta = 0) const override;

  /** energy response for the HCAL
   @param[in] energy energy
   @param[in] eta eta   */
  double energyResponse(double energy, double eta = 0) const override;

private:
  double m_clusterSize;              ///< size of cluster in HCAL
  std::vector<double> m_eresBarrel;  ///< energy resolution parameters for barrel (length 3)
  double m_eResponse;                ///< energy response
  double m_etaAcceptance;            ///< max eta for acceptance
  double m_energyAcceptance;         ///< min energy for acceptance
};

}  // end namespace papas
#endif
