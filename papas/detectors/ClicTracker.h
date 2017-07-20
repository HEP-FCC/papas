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
   * @param[in] volume The tracker cyclinders
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
  bool acceptance(const Track& track) const override;      ///< determines if a track is detected
  // TODO space_resolution(self, ptc):
private:
  double sigmaPtOverPt2(double a,double b,double pt) const;
  const double m_thetaMax;
  std::map<int, std::pair<double, double>> m_resMap;
  double m_ptThresholdLow;
  double m_ptProbabilityLow;
  double m_ptThresholdHigh;
  double m_ptProbabilityHigh;
};
} // end namespace papas
#endif
