#ifndef FIELD_H
#define FIELD_H
#include "papas/detectors/DetectorElement.h"

namespace papas {
  //Forward declaration
class Material;
class VolumeCylinder;

/**
 Detector field used to describe magnetic field
*/

class Field : public DetectorElement {
public:
  /** Constructor
   *
   * @param[in] volume The field cyclinders
   * @param[in] material field material
   * @param[in] magnitude field strength
   */
  Field(const VolumeCylinder&& volume, const Material&& material, double magnitude);
  /** Constructor
   *
   * @param[in] volume The field cyclinders
   * @param[in] material field material
   * @param[in] magnitude field strength
   */
  Field(const VolumeCylinder& volume, const Material& material, double magnitude);
  double getMagnitude() const { return m_magnitude; };

protected:
  double m_magnitude;  ///< Magentic field strength in Tesla
};

}  // end namespace papas
#endif
