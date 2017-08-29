#include "papas/detectors/cms/CMSHcal.h"

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Particle.h"
#include "papas/utility/TRandom.h"

namespace papas {

CMSHCAL::CMSHCAL(double innerRadius, double innerZ, double outerRadius, double outerZ, double clusterSize, double x0,
                 double lambdaI, double etacrack, const std::vector<std::vector<double>> eres,
                 const std::vector<std::vector<double>> eresp, const std::vector<double> acceptanceParameters)
    : Calorimeter(Layer::kHcal,
                  VolumeCylinder(Layer::kHcal, outerRadius, outerZ, innerRadius, innerZ),
                  Material("Clic_HCAL", x0, lambdaI)),
      m_clusterSize(clusterSize),
      m_etaCrack(etacrack),
      m_eres(eres),
      m_eresp(eresp),
      m_acceptanceParameters(acceptanceParameters) {}

double CMSHCAL::clusterSize(const Particle& ptc) const {
  (void)ptc;             // suppress warning messages for unused parameters;
  return m_clusterSize;  // default 0.2
}

bool CMSHCAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());
  bool accept = false;
  auto& pars = m_acceptanceParameters;

  if (eta < m_etaCrack) {
    if (energy > pars[0])
      accept = rootrandom::Random::uniform(0, 1) < (pars[1] / (1 + exp((energy + pars[2]) / (pars[3]))));
  } else if (eta < pars[4]) {
    if (energy > pars[5]) {
      if (energy < pars[6])
        accept = rootrandom::Random::uniform(0, 1) < (pars[7] + pars[8] * energy + pars[9] * (pow(energy, 2)));
      else
        accept = rootrandom::Random::uniform(0, 1) < (pars[10] / (1 + exp((energy + pars[11]) / pars[12])));
    }
  } else if (eta < pars[13] && energy > pars[14])
    accept = true;

  /*if (eta < m_etaCrack) {
    if (energy > 1.) accept = rootrandom::Random::uniform(0, 1) < (1 / (1 + exp((energy - 1.93816) / (-1.75330))));
  } else if (eta < 3.) {
    if (energy > 1.1) {
      if (energy < 10.)
        accept = rootrandom::Random::uniform(0, 1) < (1.05634 - 1.66943e-01 * energy + 1.05997e-02 * (pow(energy, 2)));
      else
        accept = rootrandom::Random::uniform(0, 1) < (8.09522e-01 / (1 + exp((energy - 9.90855) / -5.30366)));
    }
  } else if (eta < 5. && energy > 7)
    accept = true;*/

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
