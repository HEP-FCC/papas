//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DETECTOR_H
#define DETECTOR_H

#include <list>
#include "geometry.h"
#include "material.h"


namespace papas {
  
class Particle;
class Material;
class VolumeCylinder;
class Cluster;
class Track;


/**
 Detector Element is the  base for Calorimeter, Tracker and Field
*/

class DetectorElement {
public:
  /** Constructor (allows the Material and Volume to be created on the fly)
   * @param[in] layer : enum kEcal, kHcal, ktracker, kField
   * @param[in] const VolumeCylinder&& volume : defines the element cyclinders
   * @param[in] const Material&& material : material allows the Material to be created on the fly
    */
  DetectorElement(Layer layer,
                  const VolumeCylinder&& volume,
                  const Material&& material);

  /** Constructor (requires the Material and Volume to be already in existance)
   * @param[in] layer : enum kEcal, kHcal, ktracker, kField
   * @param[in] const VolumeCylinder& volume : defines the element cyclinders
   * @param[in] const Material& material : material
   */
  DetectorElement(Layer layer,
                  const VolumeCylinder& volume,
                  const Material& material);
  const VolumeCylinder& volumeCylinder() const { return m_volume; }  ///< return the volume cyclinder
  Layer layer() const { return m_layer; }; ///< returns kEcal, kHcal etc
  const Material& material() const { return m_material; }

protected:
  VolumeCylinder m_volume; ///< The cyclinders describing the detector element
  Material m_material;
  Layer m_layer; ///< eg kEcal, kHcal

private:
};

/** The Calorimeter class  is the basis of HCAL and ECAL elements. 
    It contains virtual functions that must be implemented by users so as to be detector specific. 
    Users must provide clusterSize/acceptance/energyResolution methods for the inheriting class
 */
class Calorimeter : public DetectorElement {
public:
  enum LOCATION {kBarrel = 0, kEndCap = 1};
  using DetectorElement::DetectorElement;
  
  /** energy Resolution of ECAL
   @param[in] energy : TODO
   @param[in] eta : angle of arrival
   @return minimum energy resolution of the detector
   */
  virtual double energyResolution(double energy, double eta) const = 0;
  
  /** energy response of ECAL
   @param[in] energy : TODO
   @param[in] eta : angle of arrival
   @return TODO ask COLIN
   */
  virtual double energyResponse(double energy, double  eta = 0) const = 0; 
  
  /** Minimum size that will be seen by a detector
   @param[in]  const Particle& ptc : particle that is to be detected
   @return size of resulting cluster (TODO units)
   */
  virtual double clusterSize(const Particle& ptc) const = 0;
  
  /** Decides whether a cluster will be seen by a detector
   @param[in]  the cluster to be analysed
   @return true is cluster is detected, false it if is too small to be seen
   */
  virtual bool acceptance(const Cluster& ptc) const = 0;

private:
};

class Field : public DetectorElement {
public:
  Field(const VolumeCylinder&& volume, const Material&& material,
        double magnitude);  ///< allows the Material and Volume to be created on the fly

  Field(const VolumeCylinder& volume, const Material& material, double magnitude);
  double getMagnitude() const { return m_magnitude; };

protected:
  double m_magnitude;
};

class Tracker : public DetectorElement {
public:
  using DetectorElement::DetectorElement;
  virtual double ptResolution(const Track&) const = 0;
  virtual bool acceptance(const Track&) const = 0;

protected:
};

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
} // end namespace papas
#endif
