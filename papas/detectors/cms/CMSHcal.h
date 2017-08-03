#ifndef CMSHCAL_H
#define CMSHCAL_H

#include "papas/detectors/Calorimeter.h"

#include <vector>

namespace papas {

class Particle;
class Cluster;

/// CMS specific implementation of  HCAL Detector element
///
class CMSHCAL : public Calorimeter {
public:
  /** Constructor
   *
   @param[in] innerRadius radius of inner cylinder of HCAL
   @param[in] innerZ z of inside of HCAL
   @param[in] outerRadius radius of outer cylinder of HCAL
   @param[in] outerZ z of outside of HCAL
   @param[in] X0 X0 of HCAL material
   @param[in] lambdaI lambdaI of HCAL material
   @param[in] clusterSize size of HCAL clusters
   @param[in] etaCrack eta that is on boundary between barrel and endcap
   @param[in] eres energy resolution parameters for barrel and endcap, defaults to {{0.8062, 2.753, 0.1501}, {6.803e-06,
   6.676, 0.1716}}
   @param[in] eresp energy response parameters for barrel and endcap, defaults to {{1.036, 4.452, -2.458}, {1.071, 9.471,
   -2.823}}
   @param[in] acceptanceParmaters parameters for acceptance. It is a vector of length 15, defaults to {
   1., 1., - 1.9381, -1.75330, 3., 1.1, 10., 1.05634, - 1.66943e-01, 1.05997e-02, 8.09522e-01 ,- 9.90855 , -5.30366,
   5., 7. }
   */
  CMSHCAL(double innerRadius = 1.9,
          double innerZ = 2.6,
          double outerRadius = 2.9,
          double outerZ = 3.6,
          double clusterSize = 0.2,
          double x0 = 0.,
          double lambdaI = 0.17,
          double etaCrack = 1.3,
          const std::vector<std::vector<double>> eres = {{0.8062, 2.753, 0.1501}, {6.803e-06, 6.676, 0.1716}},
          const std::vector<std::vector<double>> eresp = {{1.036, 4.452, -2.458}, {1.071, 9.471, -2.823}},
          const std::vector<double> acceptanceParameters = {1.,  // energy
                                                      1., -1.9381, -1.75330, 3., 1.1, 10., 1.05634, -1.66943e-01,
                                                      1.05997e-02, 8.09522e-01, -9.90855, -5.30366, 5., 7.});

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

  /** energy resolution for the HCAL
   @param[in] energy energy
   @param[in] eta eta   */
  double energyResolution(double energy, double eta = 0) const override;

  /** energy response for the HCAL
   @param[in] energy energy
   @param[in] eta eta   */
  double energyResponse(double energy, double eta = 0) const override;

private:
  double m_clusterSize;                        ///< size of cluster in HCAL
  double m_etaCrack;                           ///< eta divide between barrel and endcap
  std::vector<std::vector<double>> m_eres;     ///< energy resolution
  std::vector<std::vector<double>> m_eresp;    ///< energy response
  std::vector<double> m_acceptanceParameters;  ///< list of parameters used in CMS acceptance
};

}  // end namespace papas
#endif
