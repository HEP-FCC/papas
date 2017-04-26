#ifndef DETECTORELEMENT_H
#define DETECTORELEMENT_H

#include "papas/detectors/Material.h"
#include "papas/detectors/VolumeCylinder.h"

namespace papas {

/**
 Detector Element is the  base for Calorimeter, Tracker and Field
*/

class DetectorElement {
public:
  /** Constructor (allows the Material and Volume to be created on the fly)
   * @param[in] layer : enum kEcal, kHcal, kTracker, kField
   * @param[in] const VolumeCylinder&& volume : defines the element cyclinders
   * @param[in] const Material&& material : material allows the Material to be created on the fly
    */
  DetectorElement(Layer layer, const VolumeCylinder&& volume, const Material&& material);

  /** Constructor (requires the Material and Volume to be already in existance)
   * @param[in] layer : enum kEcal, kHcal, ktracker, kField
   * @param[in] const VolumeCylinder& volume : defines the element cyclinders
   * @param[in] const Material& material : material
   */
  DetectorElement(Layer layer, const VolumeCylinder& volume, const Material& material);
  
  /** Destructor*/
  virtual ~DetectorElement() = default; //needed for classes that inherit
  const VolumeCylinder& volumeCylinder() const { return m_volume; }  ///< return the volume cyclinder
  Layer layer() const { return m_layer; };                           ///< returns kEcal, kHcal etc
  const Material& material() const { return m_material; }

protected:
  VolumeCylinder m_volume;  ///< The cyclinders describing the detector element
  Material m_material;      ///< material for the detector element
  Layer m_layer;            ///< eg kEcal, kHcal

private:
};

}  // end namespace papas
#endif
