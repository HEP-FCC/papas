//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMSTRACKER_H
#define CMSTRACKER_H

#include "Tracker.h"

namespace papas {

class Track;
class VolumeCylinder;

/// CMS implementation of  Tracker
class CMSTracker : public Tracker {
public:
  
  /// constructor - allows the Material and Volume to be created on the fly
  CMSTracker(const VolumeCylinder&& volume);
  /// constructor - requires the Material and Volume to be already in existance
  CMSTracker(const VolumeCylinder& volume);
  virtual double ptResolution(const Track& track) const override;
  /// decide whether this track will be detected by the tracker
  virtual bool acceptance(const Track& track) const override;
  // TODO space_resolution(self, ptc):
private:
};

}  // end namespace papas
#endif
