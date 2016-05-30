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
 Class base for Calorimeter, Tracker and Field
*/

class DetectorElement {
public:
  DetectorElement(papas::Layer layer,
                  const VolumeCylinder&& volume,
                  const Material&& material);  ///< allows the Material and Volume to be created on the fly

  DetectorElement(papas::Layer layer,
                  const VolumeCylinder& volume,
                  const Material& material);  ///< requires the Material and Volume to be already in existance
  const VolumeCylinder& volumeCylinder() const { return m_volume; }  ///< return the volume cyclinder
  papas::Layer layer() const { return m_layer; };
  const Material& material() const { return m_material; }

protected:
  VolumeCylinder m_volume;
  Material m_material;
  papas::Layer m_layer;

private:
};

/**
 Holds virtual functions that the user must define when creating their own ECAL class
*/
class Calorimeter : public DetectorElement {
public:
  enum LOCATION {kBarrel = 0, kEndCap = 1};
  using DetectorElement::DetectorElement;
  virtual double energyResolution(double energy, double eta) const = 0;
  virtual double energyResponse(double energy, double  eta = 0) const { return 1;}
  virtual double clusterSize(const Particle& ptc) const = 0;
  virtual bool acceptance(const Cluster& ptc) const = 0;

private:
};

class Field : public DetectorElement {
public:
  Field(papas::Layer layer, const VolumeCylinder&& volume, const Material&& material,
        double magnitude);  ///< allows the Material and Volume to be created on the fly

  Field(papas::Layer layer, const VolumeCylinder& volume, const Material& material, double magnitude);
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
