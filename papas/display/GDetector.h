//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_GDETECTOR_H
#define PAPAS_GDETECTOR_H

#include "papas/detectors/Detector.h"
#include "papas/display/Drawable.h"
#include "papas/display/GDetectorElement.h"
#include <list>
#include <memory>

namespace papas {

/// Graphical representation of the detector - holds all the GDetectorElements together
class GDetector : public Drawable {
public:
  GDetector(const Detector& detector);
  ~GDetector() override = default;
  void Draw(const std::string& projection) override;
  void DrawSimple(const std::string& projection) const;  // testing
private:
  GDetectorElement m_gEcal;  ///< ECAL graphical detector element
  GDetectorElement m_gHcal;  ///< HCAL graphical detector element

  /// will contain shared_ptrs to GDetectorElements and GTrajectories to be plotted
  std::list<std::shared_ptr<Drawable>> m_drawElems;
};
}  // end namespace papas

#endif
