//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMSEcal.cc
 * @brief Implementation of the CMS ECAL  */
#include "papas/detectors/CMSEcal.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Particle.h"
#include "papas/detectors/Material.h"

namespace papas {

CMSECAL::CMSECAL(const VolumeCylinder&& volume, const Material&& material, double eta_crack, std::vector<double> emin,
                 const std::vector<std::vector<double>> eres, const std::vector<std::vector<double>> eresp)
    : Calorimeter(Layer::kEcal, volume, material), m_etaCrack(eta_crack), m_emin(emin), m_eres(eres), m_eresp(eresp) {}

double CMSECAL::clusterSize(const Particle& ptc) const {

  int pdgid = abs(ptc.pdgId());
  if ((pdgid == 22) | (pdgid == 11))
    return 0.04;
  else
    return 0.07;
}

bool CMSECAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());
  if (eta < m_etaCrack)
    return energy > m_emin[kBarrel];
  else if (eta < 2.93)
    return ((energy > m_emin[kEndCap]) & (cluster.pt() > 0.2));
  else
    return false;
}

double CMSECAL::energyResolution(double energy, double eta) const {
  int location = kBarrel;
  if (fabs(eta) > 1.479 && fabs(eta) < 3.0) location = kEndCap;  // endcap
  double stoch = m_eres[location][0] / sqrt(energy);
  double noise = m_eres[location][1] / energy;
  double constant = m_eres[location][2];
  return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}

double CMSECAL::energyResponse(double energy, double eta) const {
  int location = kBarrel;
  if (fabs(eta) > m_etaCrack) location = kEndCap;  // endcap
  // using fermi-dirac function : [0]/(1 + exp( (energy-[1]) /[2] );
  return m_eresp[location][0] / (1 + exp((energy - m_eresp[location][1]) / m_eresp[location][2]));
  return 1;
}

}  // end namespace papas
