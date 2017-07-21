/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/CMSTracker.h"

#include "papas/datatypes/Track.h"
#include "papas/utility/TRandom.h"

namespace papas {

CMSTracker::CMSTracker(double radius, double z, double x0, double lambdaI, double resolution, double ptThreshold,
                       double etaThresholdLow, double ptProbabilityLow, double etaThresholdHigh,
                       double ptProbabilityHigh)
    : Tracker(Layer::kTracker, VolumeCylinder(Layer::kTracker, radius, z), Material("void", x0, lambdaI)),
      m_resolution(resolution),
      m_ptThreshold(ptThreshold),
      m_etaThresholdLow(etaThresholdLow),
      m_ptProbabilityLow(ptProbabilityLow),
      m_etaThresholdHigh(etaThresholdHigh),
      m_ptProbabilityHigh(ptProbabilityHigh) {}

bool CMSTracker::acceptance(const Track& track) const {
  double pt = track.p3().Perp();
  double eta = fabs(track.p3().Eta());
  bool accept = false;
  if (eta < m_etaThresholdLow && pt > m_ptThreshold) {                // (eta < 1.35 && pt > 0.5)
    accept = rootrandom::Random::uniform(0, 1) < m_ptProbabilityLow;  // 0.95
  } else if (eta < m_etaThresholdHigh && pt > m_ptThreshold) {
    accept = rootrandom::Random::uniform(0, 1) < m_ptProbabilityHigh;  // 0.9  }
    return accept;
  }
  return accept;
}

double CMSTracker::resolution(const Track& track) const {
  double pt = track.p3().Perp();
  (void)pt;             // suppress unused parameter warning
  return m_resolution;  // updated on 9/16 from 5e-3;
}

}  // end namespace papas
