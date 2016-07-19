//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef FIELD_H
#define FIELD_H
#include "DetectorElement.h"
#include "VolumeCylinder.h"
#include "Material.h"
#include <list>

namespace papas {


class Material;
class VolumeCylinder;

/**
 Detector Element is the  base for Calorimeter, Tracker and Field
*/


class Field : public DetectorElement {
public:
  Field(const VolumeCylinder&& volume, const Material&& material,
        double magnitude);  ///< allows the Material and Volume to be created on the fly

  Field(const VolumeCylinder& volume, const Material& material, double magnitude);
  double getMagnitude() const { return m_magnitude; };

protected:
  double m_magnitude;
};



}  // end namespace papas
#endif
