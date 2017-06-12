#ifndef CMSTRACKER_H
#define CMSTRACKER_H

#include "papas/detectors/Tracker.h"

namespace papas {

class Track;
class VolumeCylinder;

/// CMS specific implementation of the detector Tracker
///
class CMSTracker : public Tracker {
public:
  /** Constructor
   *
   * @param[in] volume The tracker cyclinders
   */
  CMSTracker(const VolumeCylinder&& volume);
  /** Constructor
   *
   * @param[in] volume The tracker cyclinders
   */
  CMSTracker(const VolumeCylinder& volume);
  double ptResolution(const Track& track) const override;  ///< describes tracker resolution of momentum
  bool acceptance(const Track& track) const override;      ///< determines if a track is detected
  // TODO space_resolution(self, ptc):
private:
};

}  // end namespace papas
#endif
