//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DISPLAY_DETECTOR_H
#define DISPLAY_DETECTOR_H

#include "TBox.h"
#include "TColor.h"
#include "TEllipse.h"
#include "Detector.h"
#include <map>
#include <memory>
#include <string>

namespace papas {

/// This is the base class used for other elements that are to be drawn.
/// The key thing is to have the Draw element
class Drawable {
public:
  Drawable();
  virtual ~Drawable() = default;
  virtual void Draw(const std::string& projection)  = 0;

private:
};

/// Class to draw the HCAL and ECAL detectors
class GDetectorElement : public Drawable {
public:
  GDetectorElement(std::shared_ptr<const DetectorElement> de);
  GDetectorElement(double radius, double dz);
  ~GDetectorElement() override = default;
  // GDetectorElement(const DetectorElement& de);
  void Draw(const std::string& projection)  override;

private:
  /// lists of shared_pointer  to circles to be used to plot the detector element
  std::list<TEllipse> m_circles;

  /// lists of shared_pointer  to boxes to be used to plot the detector element
  std::list<TBox> m_boxes;
};

/// Graphical representation of the detector - holds all the GDetectorElements together
class GDetector : public Drawable {
public:
  GDetector(const Detector& detector);
  ~GDetector() override = default;
  void Draw(const std::string& projection)  override;
  void DrawSimple(const std::string& projection) const;  // testing
private:
  GDetectorElement m_gEcal; ///< ECAL graphical detector element
  GDetectorElement m_gHcal;///< HCAL graphical detector element

  /// will contain shared_ptrs to GDetectorElements and GTrajectories to be plotted
  std::list<std::shared_ptr<Drawable>> m_drawElems;
};
}  // end namespace papas

#endif
