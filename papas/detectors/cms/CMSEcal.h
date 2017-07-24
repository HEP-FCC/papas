#ifndef CMSECAL_H
#define CMSECAL_H

#include "papas/detectors/Calorimeter.h"

#include <vector>

namespace papas {

class Particle;
class Cluster;

/// CMS specific ECAL calorimeter implementation
///
/// CMSECAL inherits from calorimeter class and must implement clusterSize/acceptance/energyResolution etc methods

class CMSECAL : public Calorimeter {
public:
  /** Constructor
   *
   @param[in] innerRadius radius of inner cylinder of ECAL
   @param[in] innerZ z of inside of ECAL
   @param[in] outerRadius radius of outer cylinder of ECAL
   @param[in] outerZ z of inside of ECAL
   @param[in] x0  X0 of ECAL material
   @param[in] lambdaI lambdaI of ECAL material
   @param[in] clusterSizePhoton size of ECAL cluster from photon
   @param[in] clusterSize size of ECAL cluster from hadrons
   @param[in] etaCrack eta that is on boundary between barrel and endcap
   @param[in] etaAcceptanceThreshold max eta for acceptance in endcap
   @param[in] ptAcceptanceThreshold min pt for acceptance in endcap
   @param[in] etaEndcapMin min eta for detection in endcap
   @param[in] etaEndcapMax max eta for detection in endcap
   @param[in] emin minimum energies for acceptance in barrel and endcap default = {0.3, 1}
   @param[in] eres energy resolution parameters for barrel and endcap default {{4.22163e-02, 1.55903e-01, 7.14166e-03},
   {-2.08048e-01, 3.25097e-01, 7.34244e-03}},
   @param[in] eresp energy response parameters for barrel and endcap defaults to {{1.00071, -9.04973, -2.48554},
   {9.95665e-01, -3.31774, -2.11123}});
   */
  CMSECAL(double innerRadius = 1.3,
          double innerZ = 2,
          double outerRadius = 1.55,
          double outerZ = 2.1,
          double x0 = 8.9e-3,
          double lambdaI = 0.275,
          double clusterSizePhoton = 0.04,
          double clusterSize = 0.07,
          double etaCrack = 1.479,
          double etaAcceptanceThreshold = 2.93,
          double ptAcceptanceThreshold = 0.2,
          double etaEndcapMin = 1.479,
          double etaEndcapMax = 3.,
          std::vector<double> emin = {0.3, 1},  // emin barrel and endcap
          std::vector<std::vector<double>> eres = {{4.22163e-02, 1.55903e-01, 7.14166e-03},
                                                   {-2.08048e-01, 3.25097e-01, 7.34244e-03}},
          std::vector<std::vector<double>> eresp = {{1.00071, -9.04973, -2.48554}, {9.95665e-01, -3.31774, -2.11123}});

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

  /** Energy response for CMS ECAL
   */
  double energyResponse(double energy = 0, double eta = 0) const override;

private:
  double m_etaCrack;                         ///< eta that forms boundary between barrel and encap
  double m_clusterSizePhoton;                ///< size of cluster from Photon
  double m_clusterSize;                      ///< size of cluster from other particles
  double m_etaAcceptanceThreshold;           ///< max eta for acceptance in endcap
  double m_ptAcceptanceThreshold;            ///< min pt for acceptance in endcap
  double m_etaEndcapMin;                     ///< min eta for detection in endcap
  double m_etaEndcapMax;                     ///< max eta for detection in endcap
  std::vector<double> m_emin;                ///< vector min energy for detection (Barrel and EndCap) length 2
  std::vector<std::vector<double>> m_eres;   ///< energy resolution parameters(Barrel and EndCap) each of 3 elements
  std::vector<std::vector<double>> m_eresp;  ///< energy response parameters (Barrel and EndCap) each of 3 elements
};

}  // end namespace papas
#endif
