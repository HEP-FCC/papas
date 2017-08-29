/**
 * @file CMSEcal.cc
 * @brief Implementation of the CMS ECAL  */
#include "papas/detectors/cms/CMSEcal.h"

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Particle.h"

namespace papas {

CMSECAL::CMSECAL(double innerRadius,
                 double innerZ,
                 double outerRadius,
                 double outerZ,
                 double x0,
                 double lambdaI,
                 double clusterSizePhoton,
                 double clusterSize,
                 double etaCrack,
                 double etaAcceptanceThreshold,
                 double ptAcceptanceThreshold,
                 double etaEndcapMin,
                 double etaEndcapMax,
                 const std::vector<double>
                     emin,
                 const std::vector<std::vector<double>>
                     eres,
                 const std::vector<std::vector<double>>
                     eresp)
    : Calorimeter(Layer::kEcal,
                  VolumeCylinder(Layer::kEcal, outerRadius, outerZ, innerRadius, innerZ),
                  Material("CMS_ECAL", x0, lambdaI)),
      m_etaCrack(etaCrack),
      m_clusterSizePhoton(clusterSizePhoton),
      m_clusterSize(clusterSize),
      m_etaAcceptanceThreshold(etaAcceptanceThreshold),
      m_ptAcceptanceThreshold(ptAcceptanceThreshold),
      m_etaEndcapMin(etaEndcapMin),
      m_etaEndcapMax(etaEndcapMax),
      m_emin(emin),
      m_eres(eres),
      m_eresp(eresp) {}

double CMSECAL::clusterSize(const Particle& ptc) const {
  int pdgid = abs(ptc.pdgId());
  if ((pdgid == 22) | (pdgid == 11))
    return m_clusterSizePhoton;
  else
    return m_clusterSize;
}

bool CMSECAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());
  if (eta < m_etaCrack)
    return energy > m_emin[kBarrel];
  else if (eta < m_etaAcceptanceThreshold)                                           // 2.93)
    return ((energy > m_emin[kEndCap]) & (cluster.pt() > m_ptAcceptanceThreshold));  // 0.2));
  else
    return false;
}

double CMSECAL::energyResolution(double energy, double eta) const {
  int location = kBarrel;
  if (fabs(eta) > m_etaEndcapMin && fabs(eta) < m_etaEndcapMax) location = kEndCap;  // endcap
  // if (fabs(eta) > 1.479 && fabs(eta) < 3.0) location = kEndCap;
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
}

}  // end namespace papas
