//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef TRACKER_H
#define TRACKER_H

#include "papas/detectors/DetectorElement.h"
#include "papas/detectors/Material.h"
#include <list>

namespace papas {

class Track;

///< Base class for tracker detector element. User must define own tracker.
class Tracker : public DetectorElement {
public:
  using DetectorElement::DetectorElement;
  /// User defined function - limit of momentum that can be distinguished by tracker
  virtual double ptResolution(const Track&) const = 0;
  virtual bool acceptance(const Track&) const = 0;

protected:
};

}  // end namespace papas
#endif
