#ifndef ClicECAL_H
#define ClicECAL_H

#include "papas/detectors/Calorimeter.h"

#include <vector>

namespace papas {

class Particle;
class Cluster;
class Material;
class VolumeCylinder;

/// Clic specific ECAL calorimeter implementation
///
///  It is intended to be replicated/modfied by users to match other required detector characteristics
///
/// ClicECAL inherits from calorimeter class and must implement clusterSize/acceptance/energyResolution etc methods

class ClicECAL : public Calorimeter {
public:
  /** Constructor
   *
   * @param[in] volume The ECAL cyclinders
   * @param[in] material ECAL material
   * @param[in] eta_crack ask Colin
   * @param[in] emin vector of minimum energy { barrel, endcap}
   * @param[in] eres vector of  vectors of energy resolution { barrel, endcap}
   */
  ClicECAL(double innerRadius = 2.15,
           double innerZ = 2.6,
           double depth = .25,
           double clusterSizePhoton = .015,
           double clusterSize = 0.045,
           double etaAcceptance = 2.76,
           double eminBarrel = 0.5,
           double eminEndcap = 0.5,
           std::vector<double> eresBarrel = {0.165, 0.010, 0.015},
           int nX0 = 23,
           int nLambdaI = 1,
           double eResponse = 1.);

  /** Minimum size that will be seen by a detector
   @param[in]  ptc  particle that is to be detected
   @return minimum size of cluster that can be seen (TODO units)
   */
  double clusterSize(const Particle& ptc) const override;

  /** Decides whether a cluster will be seen by a detector
   @param[in]  cluster the cluster to be analysed
   @return true if cluster is detected, false it if is too small to be seen
   */
  bool acceptance(const Cluster& cluster) const override;

  /** energy Resolution of ECAL
   @param[in] energy
   @param[in] eta angle of arrival
   @return minimum energy resolution of the detector
   */
  double energyResolution(double energy, double eta = 0) const override;

  /** TODO ask Colin for comment details
   */
  double energyResponse(double energy = 0, double eta = 0) const override;

  // TODOAJR space_resolution(self, ptc):
private:
  double m_clusterSize;        ///< cluater size for other particles
  double m_clusterSizePhoton;  ///<cluster size for photon and electron
  double m_eminBarrel;
  double m_eminEndcap;
  std::vector<double> m_eresBarrel;
  double m_eResponse;  ///<energy response
  double m_etaAcceptance;
};

}  // end namespace papas
#endif
