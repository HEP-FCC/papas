//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMSHCAL_H
#define CMSHCAL_H

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

class CMSHCAL : public Calorimeter {
public:
  // TODO consider best approach for below?
  /// constructor - allows the Material and Volume to be created on the fly
  CMSHCAL(const VolumeCylinder&& volume,
          const Material&& material,
          double m_eta_crack,
          std::vector<std::vector<double>>
              eres,
          std::vector<std::vector<double>>
              eresp);
  /// constructor - requires the Material and Volume to be already in existance
  CMSHCAL(const VolumeCylinder& volume,
          const Material& material,
          double m_etaCrack,
          std::vector<std::vector<double>>
              eres,
          std::vector<std::vector<double>>
              eresp);

  double clusterSize(const Particle& ptc) const override;
  bool acceptance(const Cluster& cluster) const override;
  double energyResolution(double energy, double eta = 0) const override;
  double energyResponse(double energy, double eta = 0) const override;
  // TODOAJR space_resolution(self, ptc):
private:
  double m_etaCrack;
  std::vector<std::vector<double>> m_eres;   /// energy resolution
  std::vector<std::vector<double>> m_eresp;  /// ask Colin what this is};
};

}  // end namespace papas
#endif
