//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "CMSHcal.h"
#include "Cluster.h"
#include "PParticle.h"
#include "Path.h"
#include "Random.h"

namespace papas {

CMSHCAL::CMSHCAL(const VolumeCylinder& volume, const Material& material, double etacrack,
                 std::vector<std::vector<double>> eres, std::vector<std::vector<double>> eresp)
    : Calorimeter(Layer::kHcal, volume, material), m_etaCrack(etacrack), m_eres(eres), m_eresp(eresp) {}

CMSHCAL::CMSHCAL(const VolumeCylinder&& volume, const Material&& material, double etacrack,
                 std::vector<std::vector<double>> eres, std::vector<std::vector<double>> eresp)
    : Calorimeter(Layer::kHcal, volume, material), m_etaCrack(etacrack), m_eres(eres), m_eresp(eresp) {}

// will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double CMSHCAL::clusterSize(const Particle& ptc) const {
  (void)ptc;  // suppress warning messages for unused parameters;
  return 0.2;
}

/* Decides whether a cluster will be seen by a detector
 @param cluster the cluster to be analysed
 @return true is cluster is detected, false it if is too small to be seen
 */
bool CMSHCAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());

  bool accept = false;
  randomgen::RandUniform rUniform{0, 1};
  if (eta < m_etaCrack) {
    if (energy > 1.) accept = rUniform.next() < (1 / (1 + exp((energy - 1.93816) / (-1.75330))));
  } else if (eta < 3.) {
    if (energy > 1.1) {
      if (energy < 10.)
        accept = rUniform.next() < (1.05634 - 1.66943e-01 * energy + 1.05997e-02 * (pow(energy, 2)));
      else
        accept = rUniform.next() < (8.09522e-01 / (1 + exp((energy - 9.90855) / -5.30366)));
    }
  } else if (eta < 5. && energy > 7)
    accept = true;

  return accept;
}

double CMSHCAL::energyResolution(double energy, double eta) const {
  int part = kBarrel;
  if (fabs(eta) > m_etaCrack) part = kEndCap;
  double stoch = m_eres[part][0] / sqrt(energy);
  double noise = m_eres[part][1] / energy;
  double constant = m_eres[part][2];
  return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}

double CMSHCAL::energyResponse(double energy, double eta) const {
  int part = kBarrel;
  if (fabs(eta) > m_etaCrack) part = kEndCap;
  // using fermi-dirac function : [0]/(1 + exp( (energy-[1]) /[2] ))
  return m_eresp[part][0] / (1 + exp((energy - m_eresp[part][1]) / m_eresp[part][2]));
}

}  // end namespace papas
