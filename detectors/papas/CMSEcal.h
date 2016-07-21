//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMSECAL_H
#define CMSECAL_H

#include "Calorimeter.h"
#include <vector>

namespace papas {

class Particle;
class Cluster;
class Material;
class VolumeCylinder;
/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */

/// CMSECAL Class (CMS implementation)
/** This ECAL is implmented specifically for CMS

      It is intended to be replaced by users to match the required detector characteristics

   CMSECAL inherits from calorimeter class and must implement clusterSize/acceptance/energyResolution etc methods

 */
//
class CMSECAL : public Calorimeter {
public:
  // AJRTODO check whether both constructors are needed
  /** Constructor
   *
   * @param[in] const VolumeCylinder&& volume : defines the ECAL cyclinder
   * @param[in] const Material&& material : ECAL material
   * @param[in] double eta_crack : ask Colin
   * @param[in] std::vector<double> emin : TODO minimum energy for barrel and endcap
   * @param[in] std::vector<std::vector<double>> eres : TODO
   */
  CMSECAL(const VolumeCylinder&& volume, const Material&& material, double eta_crack, std::vector<double> emin,
          std::vector<std::vector<double>> eres);

  /// constructor - requires the Material and Volume to be already in existance
  /*ECAL(papas::Layer layer, const VolumeCylinder& volume,
       const Material& material , double eta_crack, double emin,
       const std::vector<double>& eres);*/

  /** Minimum size that will be seen by a detector
   @param[in]  const Particle& ptc : particle that is to be detected
   @return minimum size of cluster that can be seen (TODO units)
   */
  double clusterSize(const Particle& ptc) const override;

  /** Decides whether a cluster will be seen by a detector
   @param[in]  the cluster to be analysed
   @return true if cluster is detected, false it if is too small to be seen
   */
  bool acceptance(const Cluster& cluster) const override;

  /** energy Resolution of ECAL
   @param[in] energy : TODO
   @param[in] eta : angle of arrival
   @return minimum energy resolution of the detector
   */
  double energyResolution(double energy, double eta = 0) const override;

  /** ask Colin for details
   */
  double energyResponse(double energy = 0, double eta = 0) const override;

  // TODOAJR space_resolution(self, ptc):
private:
  double m_etaCrack;                        ///< ask Colin
  std::vector<double> m_emin;               ///< contains two elements (Barrel and EndCap)
  std::vector<std::vector<double>> m_eres;  ///< two vectors (Barrel and EndCap) each of 3 elements
                                            ///> TODO describe elements
};

}  // end namespace papas
#endif
