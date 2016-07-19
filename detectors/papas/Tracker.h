//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef TRACKER_H
#define TRACKER_H

#include "DetectorElement.h"
#include "Material.h"
#include <list>

namespace papas {


class Track;


class Tracker : public DetectorElement {
public:
  using DetectorElement::DetectorElement;
  virtual double ptResolution(const Track&) const = 0;
  virtual bool acceptance(const Track&) const = 0;

protected:
};

}  // end namespace papas
#endif
