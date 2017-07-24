/**
 * @file ClicEcal.cc
 * @brief Implementation of the Clic ECAL  */
#include "papas/detectors/clic/ClicEcal.h"

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Particle.h"

namespace papas {

ClicECAL::ClicECAL(double innerRadius,
                   double innerZ,
                   double depth,
                   double clusterSizePhoton,
                   double clusterSize,
                   double etaAcceptance,
                   std::vector<double> emin,
                   std::vector<double>
                       eresBarrel,
                   int nX0,
                   int nLambdaI,
                   double eResponse)
    : Calorimeter(Layer::kEcal,
                  VolumeCylinder(Layer::kEcal, innerRadius + depth, innerZ + depth, innerRadius, innerZ),
                  Material("Clic_ECAL", depth / nX0, depth / nLambdaI)),
      m_clusterSize(clusterSize),
      m_clusterSizePhoton(clusterSizePhoton),
      m_emin(emin),
      m_eresBarrel(eresBarrel),
      m_eResponse(eResponse),
      m_etaAcceptance(etaAcceptance) {}

double ClicECAL::clusterSize(const Particle& ptc) const {
  int pdgid = abs(ptc.pdgId());
  if ((pdgid == 22) | (pdgid == 11))
    return m_clusterSizePhoton;  // 0.015;
  else
    return m_clusterSize;  // 0.045;
}

bool ClicECAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());
  if (eta < volumeCylinder().inner().etaJunction())
    return (energy > m_emin[0]); //barrel
  else if (eta < m_etaAcceptance)
    return energy > m_emin[1]; //endcap
  else
    return false;
}

double ClicECAL::energyResolution(double energy, double eta) const {
  double stoch = m_eresBarrel[0] / sqrt(energy);
  double noise = m_eresBarrel[1] / energy;
  double constant = m_eresBarrel[2];
  return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}

double ClicECAL::energyResponse(double energy, double eta) const { return m_eResponse; }

}  // end namespace papas
