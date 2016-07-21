//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DETECTOR_H
#define DETECTOR_H

#include <memory>

#include "Calorimeter.h"
#include "DetectorElement.h"
#include "Field.h"
#include "SurfaceCylinder.h"
#include "Tracker.h"

namespace papas {

/// Detector
/**
   Class from which user can provide their own detector code
   It must contain an HCAL, ECAL, Tracker and Field element
 */

class Detector {
public:
  Detector();

  const std::list<SurfaceCylinder>& sortedCylinders();  /// AJRTODO make this sort on initialisation
  std::shared_ptr<const DetectorElement> element(papas::Layer layer) const;
  std::shared_ptr<const Calorimeter> calorimeter(papas::Layer layer) const;
  std::shared_ptr<const Calorimeter> ecal() const { return m_ecal; }
  std::shared_ptr<const Calorimeter> hcal() const { return m_hcal; }
  std::shared_ptr<const Tracker> tracker() const { return m_tracker; }
  std::shared_ptr<const Field> field() const { return m_field; };

protected:
  // shared pointers allow user to have their own derived ECAL and HCAL calorimeter class
  // that has a fixed interface defined by Calorimeter
  std::shared_ptr<const class Calorimeter> m_ecal;
  std::shared_ptr<const class Calorimeter> m_hcal;
  std::shared_ptr<const Tracker> m_tracker;
  std::shared_ptr<const Field> m_field;

private:
  std::list<SurfaceCylinder> m_cylinders;  // or use pointers here?
};
}  // end namespace papas
#endif
