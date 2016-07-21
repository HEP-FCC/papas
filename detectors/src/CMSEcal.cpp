//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMSEcal.cc
 * @brief Implementation of the CMS ECAL  */
#include "CMSEcal.h"
#include "Cluster.h"
#include "Material.h"
#include "PParticle.h"

namespace papas {
/*
 CMSECAL::CMSECAL(papas::Layer layer, const VolumeCylinder& volume,
 const  Material& material , double eta_crack,
 double emin, const std::vector<double>& eres) :
 DetectorElement(layer,   volume,  material), m_eta_crack(eta_crack), m_emin(emin),
 m_eres(eres)
 {}*/

CMSECAL::CMSECAL(const VolumeCylinder&& volume, const Material&& material, double eta_crack, std::vector<double> emin,
                 const std::vector<std::vector<double>> eres)
    : Calorimeter(Layer::kEcal, volume, material), m_etaCrack(eta_crack), m_emin(emin), m_eres(eres) {}

// will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
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
  (void)energy;  // suppress warning messages for unused parameters;
  (void)eta;
  return 1;
}

}  // end namespace papas
