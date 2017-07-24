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
/// ClicECAL inherits from calorimeter class and must implement clusterSize/acceptance/energyResolution etc methods

class ClicECAL : public Calorimeter {
public:
  /** Constructor
   *
   @param[in] innerRadius radius of inner cylinder of ECAL
   @param[in] innerZ z of inside of ECAL
   @param[in] depth of ECAL cylinder
   @param[in] clusterSizePhoton size of ECAL cluster from photon
   @param[in] clusterSize size of ECAL cluster from hadrons
   @param[in] etaAcceptance max eta for acceptance in endcap
   @param[in] emin minimum energies for acceptance in barrel and endcap default = {0.5, 0.5}
   @param[in] eresBarrel energy resolution parameters for barrel  default   {0.165, 0.010, 0.015}
   @param[in] nX0 number of X0 for material in ECAL
   @param[in] nLambdaI number of lambdaI for material in ECAL
   @param[in] eResponse energy response;
   */
  ClicECAL(double innerRadius = 2.15,
           double innerZ = 2.6,
           double depth = .25,
           double clusterSizePhoton = .015,
           double clusterSize = 0.045,
           double etaAcceptance = 2.76,
           std::vector<double> emin = {0.5, 0.5},
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

  /** Energy response
   @param[in] energy
   @param[in] eta angle of arrival
   @return energy response of the ECAL
   */
  double energyResponse(double energy = 0, double eta = 0) const override;

private:
  double m_clusterSize;              ///< cluster size for particles otehr than photon and electron
  double m_clusterSizePhoton;        ///< cluster size for photon and electron
  std::vector<double> m_emin;        ///< min energy for acceptance in barrel and endcap
  std::vector<double> m_eresBarrel;  ///< energy resolution paremeters for barrel
  double m_eResponse;                ///< energy response
  double m_etaAcceptance;            ///< max eta for detection in endcap
};

}  // end namespace papas
#endif
