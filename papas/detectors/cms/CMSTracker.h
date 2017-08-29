#ifndef CMSTRACKER_H
#define CMSTRACKER_H

#include "papas/detectors/Tracker.h"

namespace papas {

class Track;

/// CMS specific implementation of the detector Tracker
///
class CMSTracker : public Tracker {
public:
  /** Constructor
   *
   @param[in] radius radius of tracker
   @param[in] z z of tracker
   @param[in] x0 X0 of tracker material
   @param[in] lambdaI lambdaI of tracker material
   @param[in] resolution resolution of tracker
   @param[in] ptThreshold minimum pt for acceptance
   @param[in] etaThresholdLow lower eta threshold
   @param[in] ptProbabilityLow lower eta acceptance probabilty
   @param[in] etaThresholdHigh upper eta threshold
   @param[in] ptProbabilityHigh upper eta acceptance probabilty
   */
  CMSTracker(double radius = 1.29,
             double z = 1.99,
             double x0 = 0.,
             double lambdaI = 0.,
             double resolution = 1.1e-2,
             double ptThreshold = 0.5,
             double etaThresholdLow = 1.35,
             double ptProbabilityLow = 0.95,
             double etaThresholdHigh = 2.5,
             double ptProbabilityHigh = 0.9);
  double resolution(const Track& track) const override;  ///< describes tracker resolution of momentum
  bool acceptance(const Track& track) const override;    ///< determines if a track is detected
private:
  double m_resolution;         ///< resolution of tracker
  double m_ptThreshold;        ///< minimum pt for acceptance
  double m_etaThresholdLow;    ///< lower eta threshold
  double m_ptProbabilityLow;   ///< lower eta acceptance probabilty
  double m_etaThresholdHigh;   ///< upper eta threshold
  double m_ptProbabilityHigh;  ///< upper eta acceptance probabilty
};

}  // end namespace papas
#endif
