#ifndef PAPAS_GDETECTORELEMENT_H
#define PAPAS_GDETECTORELEMENT_H

#include <list>
#include <memory>
#include <string>

#include "TBox.h"
#include "TEllipse.h"

#include "papas/display/Drawable.h"

namespace papas {

 //Forward Declaration
class DetectorElement;

/// Class to draw the HCAL and ECAL detectors
class GDetectorElement : public Drawable {
public:
  GDetectorElement(std::shared_ptr<const DetectorElement> de);
  GDetectorElement(double radius, double dz);
  ~GDetectorElement() override = default;
  void Draw(const std::string& projection) override;

private:
  /// lists of shared_pointer  to circles to be used to plot the detector element
  std::list<TEllipse> m_circles;

  /// lists of shared_pointer  to boxes to be used to plot the detector element
  std::list<TBox> m_boxes;
};

}  // end namespace papas

#endif
