//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DETECTOR_H
#define DETECTOR_H


#include <string>
#include <unordered_map>
#include <list>
#include <memory>
#include "material.h"
#include "geometry.h"

class Particle;
class Material;
class VolumeCylinder;
class Cluster;
///DetectorElement
/**
 Class base for ECAL, HCAL, Track and field
*/

class DetectorElement {
public:
   DetectorElement(const std::string& name, const VolumeCylinder&& volume,
                   const Material&&
                   material); ///< allows the Material and Volume to be created on the fly

   DetectorElement(const std::string& name, const VolumeCylinder&  volume,
                   const Material&
                   material); ///< requires the Material and Volume to be already in existance
   //AJRTODO assumes a volume will not change once created which seems reasonable
   const VolumeCylinder* getVolume() const   ///< return the volume cyclinder
   {
      return &m_volume;
   };
   const std::string getName() const {return m_name;};
protected:
   VolumeCylinder m_volume;
   Material m_material;
   std::string m_name;
private:
};

///BaseECAL
/**
 Holds virtual functions that the user must define when creating their own ECAL class
*/
class BaseECAL: public DetectorElement {
public:
   using DetectorElement::DetectorElement;

   virtual double energy_resolution(double energy) const = 0;
   virtual double cluster_size(const Particle& ptc) const = 0 ;
   virtual bool acceptance(const Cluster&) const = 0;
   //virtual double space_resolution(Particle* ptc)=0;
private:

};


///BaseECAL
/**
 Holds virtual functions that the user must define when creating their own ECAL class
 */
class BaseHCAL: public DetectorElement {
public:
   using DetectorElement::DetectorElement;
   
   virtual double energy_resolution(double energy) const = 0;
   virtual double cluster_size(const Particle& ptc) const = 0 ;
   virtual bool acceptance(const Cluster&) const = 0;
   //virtual double space_resolution(Particle* ptc)=0;
private:
   
};

///BaseDetector
/**
   Class from which user can provide their own detector code
   It must contain an HCAL, ECAL, Tracker and Field element
 */
class BaseDetector {
public:
   BaseDetector();
   const std::list<SurfaceCylinder>  getSortedCylinders(); ///AJRTODO make this simply return the list (or a copy) - sort on                                initialisation
   std::shared_ptr<const DetectorElement> getElement(const std::string&) const;
protected:
   //AJRTODO may replace this with explicit HCAL, ECAL etc
   std::unordered_map<std::string, std::shared_ptr<const DetectorElement>>
         m_detectorElements;
private:
   std::list<SurfaceCylinder> m_cylinders; // or use pointers here?

};

#endif