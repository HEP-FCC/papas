#include "papas/detectors/clic/ClicTracker.h"

#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/utility/TRandom.h"

#include <cmath>
#include <map>

namespace papas {

ClicTracker::ClicTracker(double radius, double z, double x0, double lambdaI, double thetaParam,
                         const std::map<int, std::pair<double, double>> resMap, double ptThresholdLow,
                         double ptProbabilityLow, double ptThresholdHigh, double ptProbabilityHigh)
    : Tracker(Layer::kTracker, VolumeCylinder(Layer::kTracker, radius, z), Material("Clic_Tracker", x0, lambdaI)),
      m_thetaMax(thetaParam * M_PI / 180.),  // 80
      m_resMap(resMap),                      // m_resMap = {{90, {8.2e-2, 9.1e-2}},
                                             // {80, {8.2e-4, 9.1e-3}},
                                             // {30, {9.9e-5, 3.8e-3}},
                                             // {20, {3.9e-5, 1.6e-3}},
                                             // {10, {2e-5, 7.2e-4}}
      m_ptThresholdLow(ptThresholdLow),
      m_ptProbabilityLow(ptProbabilityLow),
      m_ptThresholdHigh(ptThresholdHigh),
      m_ptProbabilityHigh(ptProbabilityHigh){};

bool ClicTracker::acceptance(const Track& track) const {
  double pt = track.p3().Perp();
  double theta = fabs(track.theta());
  if (theta < m_thetaMax) {
    if (pt > m_ptThresholdLow)  // 0.4)
      return (rootrandom::Random::uniform(0, 1) < m_ptProbabilityLow);
    else if (pt > m_ptThresholdHigh)  // 2)
      return (rootrandom::Random::uniform(0, 1) < m_ptProbabilityHigh);
  }
  return false;
}

double ClicTracker::sigmaPtOverPt2(double a, double b, double pt) const {
  /// CLIC CDR Eq. 5.1'''
  return sqrt(a * a + pow(b / pt, 2));
}

double ClicTracker::resolution(const Track& track) const {
  /*Returns relative resolution on the track momentum
  CLIC CDR, Table 5.3
  */
  double pt = track.p3().Pt();
  // matching the resmap defined above.
  double theta = fabs(track.theta()) * 180 / M_PI;
  for (const auto& v : m_resMap) {
    if (theta < v.first) return sigmaPtOverPt2(v.second.first, v.second.second, pt) * pt;
  }
  throw "tracker resolution not found";
  return 0;
}

double ClicTracker::resolution(const Particle& ptc) const {
  // not very nice due to particle being fed through into a track parameter in python
  // will do for now
  double pt = ptc.p3().Pt();
  // matching the resmap defined above.
  double theta = fabs(ptc.theta()) * 180 / M_PI;
  for (const auto& v : m_resMap) {
    if (theta < v.first) return sigmaPtOverPt2(v.second.first, v.second.second, pt) * pt;
  }
  throw "tracker particle resolution not found";
  return 0;
}

bool ClicTracker::electronAcceptance(double mag, const Track& track) const {
  if (track.p3().Pt() > mag && fabs(track.theta()) < m_thetaMax) return rootrandom::Random::uniform(0, 1) > 0.95;
  return false;
}

  
  

}  // end namespace papas
