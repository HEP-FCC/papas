#ifndef ClicFIELD_H
#define ClicFIELD_H

#include "papas/detectors/Field.h"

namespace papas {

// Forward declaration
class VolumeCylinder;
/// Clic specific implementation of Detector Field element
///
class ClicField : public Field {
public:
  /** Constructor
   *
   
   * @param[in] magnitude field strength
   * @param[in] field cyclinder radius
   * @param[in] field cyclinder z
   */
  ClicField(double magnitude, double radius , double z);
  
private:
};

}  // end namespace papas
#endif
