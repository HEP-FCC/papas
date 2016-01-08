//
//  Created by Alice Robson on 09/11/15.
//
//
#include <vector>
#include <list>
#include "detector.h"
#include "material.h"
#include "geometry.h"



DetectorElement::DetectorElement(const std::string& name,
                                 const  VolumeCylinder& volume , const Material& material) :
   m_volume(volume), m_material(material), m_name(name)
{}



BaseDetector::BaseDetector() : m_cylinders()
{
}

//aim to make this a const function and to do sort at initialisation
const std::list<SurfaceCylinder> BaseDetector::getSortedCylinders()
{
   //Return list of surface cylinders sorted by increasing radius.'''

   return m_cylinders;
   /* for element in self.elements.values():
    if element.volume.inner is not None:
    self._cylinders.append(element.volume.inner)
    self._cylinders.append(element.volume.outer)
    self._cylinders.sort(key=operator.attrgetter("rad"))
   return m_cylinders;*/
}

std::shared_ptr<const DetectorElement> BaseDetector::getElement(
   const std::string& elemname) const
{
   return m_detectorElements.find(elemname)->second;
}



/*

BaseECAL::BaseECAL(const std::string& name, const VolumeCylinder& volume, const Material& material) :
   DetectorElement(name, volume, material)
{

}


BaseECAL::BaseECAL(const std::string& name, const VolumeCylinder&& volume, const Material&& material) :
   DetectorElement(name, volume, material)
{

}*/



/*
BaseECAL::BaseECAL(const std::string& name, const VolumeCylinder& volume, const Material& material, double eta_crack, double emin, const std::vector<double>& eres) :
DetectorElement(name, volume, material),
m_eta_crack(eta_crack),
m_emin(emin),
m_eres(eres)
{

}

BaseECAL::BaseECAL(const std::string& name,const VolumeCylinder&& volume, const Material&& material, double eta_crack, double emin,const std::vector<double> &&eres) :
DetectorElement(name, volume, material),
m_eta_crack(eta_crack),
m_emin(emin),
m_eres(eres)
{

}

double BaseECAL::energy_resolution(double energy) const
{
    double stoch = m_eres[0] / sqrt(energy);
    double noise = m_eres[1] / energy;
    double constant = m_eres[2];
    return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}

*/
