#include "papas/detectors/clic/ClicHcal.h"

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Particle.h"

namespace papas {

ClicHCAL::ClicHCAL(double innerRadius, double innerZ, double outerRadius, double outerZ, double clusterSize,
                   const std::vector<double> eresBarrel, double x0, double lambdaI, double eResponse, double etaAcceptance,
                   double energyAcceptance)
    : Calorimeter(Layer::kHcal,
                  VolumeCylinder(Layer::kHcal, outerRadius, outerZ, innerRadius, innerZ),
                  Material("Clic_HCAL", x0, lambdaI)),

      m_clusterSize(clusterSize),
      m_eresBarrel(eresBarrel),
      m_eResponse(eResponse),
      m_etaAcceptance(etaAcceptance),
      m_energyAcceptance(energyAcceptance) {}

double ClicHCAL::clusterSize(const Particle& ptc) const {
  (void)ptc;  // suppress warning messages for unused parameters;
  return m_clusterSize;
}

bool ClicHCAL::acceptance(const Cluster& cluster) const {
  auto energy = cluster.energy();
  auto eta = fabs(cluster.position().Eta());
  if (eta < m_etaAcceptance)  
    return (energy > m_energyAcceptance);  // 1.
  else
    return false;
}

double ClicHCAL::energyResolution(double energy, double eta) const {
  double stoch = m_eresBarrel[0] / sqrt(energy);
  double noise = m_eresBarrel[1] / energy;
  double constant = m_eresBarrel[2];
  return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}

double ClicHCAL::energyResponse(double energy, double eta) const { return m_eResponse; }

}  // end namespace papas
