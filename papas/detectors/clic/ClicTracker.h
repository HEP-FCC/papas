#ifndef ClicTRACKER_H
#define ClicTRACKER_H

#include "papas/detectors/Tracker.h"
#include <map>

namespace papas {

class Track;
class VolumeCylinder;

/// Clic specific implementation of the detector Tracker
///
class ClicTracker : public Tracker {
public:
  /** Constructor
   *
   @param[in] radius radius of tracker
   @param[in] z z of itracker
   @param[in] x0  X0 of tracker material
   @param[in] lambdaI lambdaI of tracker material
   @param[in] thetaParam used to determine max value of theta for acceptance
   @param[in] resMap map between angle and response parameters used to find track resolution
   @param[in] ptThresholdLow lower pt threshold
   @param[in] ptProbabilityLow  lower threshold acceptance probabilty
   @param[in] ptThresholdHigh upper pt threshold
   @param[in] ptProbabilityHigh upper threshold acceptance probabilty
   */
  ClicTracker(double radius = 2.14,
              double z = 2.6,
              double x0 = 0,
              double lambdaI = 0,
              double thetaParam = 0.8,
              std::map<int, std::pair<double, double>> resMap = {{90, {8.2e-2, 9.1e-2}},
                                                                 {80, {8.2e-4, 9.1e-3}},
                                                                 {30, {9.9e-5, 3.8e-3}},
                                                                 {20, {3.9e-5, 1.6e-3}},
                                                                 {10, {2e-5, 7.2e-4}}},
              double ptThresholdLow = 0.4,
              double ptProbabilityLow = 0.95,
              double ptThresholdHigh = 2.,
              double ptProbabilityHigh = 0.99);
  double resolution(const Track& track) const override;  ///< describes tracker resolution of momentum
  bool acceptance(const Track& track) const override;    ///< determines if a track is detected
private:
  double sigmaPtOverPt2(double a, double b, double pt) const;
  const double m_thetaMax;  ///< max theta for acceptance
  std::map<int, std::pair<double, double>>
      m_resMap;                ///<  map between angle and response parameters used to find track resolution
  double m_ptThresholdLow;     ///< lower pt threshold
  double m_ptProbabilityLow;   ///< lower threshold acceptance probabilty
  double m_ptThresholdHigh;    ///< upper pt threshold
  double m_ptProbabilityHigh;  ///< upper threshold acceptance probabilty
};
}  // end namespace papas
#endif
