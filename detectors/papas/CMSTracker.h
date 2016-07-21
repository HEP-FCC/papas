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

class CMSTracker : public Tracker {
public:
  // TODO consider best approach for below?
  /// constructor - allows the Material and Volume to be created on the fly
  CMSTracker(const VolumeCylinder&& volume);
  /// constructor - requires the Material and Volume to be already in existance
  CMSTracker(const VolumeCylinder& volume);
  virtual double ptResolution(const Track& track) const override;
  virtual bool acceptance(const Track& track) const override;
  // TODOAJR space_resolution(self, ptc):
private:
};

}  // end namespace papas
#endif
