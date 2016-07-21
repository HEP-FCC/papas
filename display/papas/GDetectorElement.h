//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_GDETECTORELEMENT_H
#define PAPAS_GDETECTORELEMENT_H

#include <list>
#include <memory>
#include <string>

#include "DetectorElement.h"
#include "Drawable.h"
#include "TBox.h"
#include "TEllipse.h"

namespace papas {

/// Class to draw the HCAL and ECAL detectors
class GDetectorElement : public Drawable {
public:
  GDetectorElement(std::shared_ptr<const DetectorElement> de);
  GDetectorElement(double radius, double dz);
  ~GDetectorElement() override = default;
  // GDetectorElement(const DetectorElement& de);
  void Draw(const std::string& projection) override;

private:
  /// lists of shared_pointer  to circles to be used to plot the detector element
  std::list<TEllipse> m_circles;

  /// lists of shared_pointer  to boxes to be used to plot the detector element
  std::list<TBox> m_boxes;
};

}  // end namespace papas

#endif
