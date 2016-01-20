//
//  Created by Alice Robson on 09/11/15.
//
//
#include <vector>
#include <list>
#include "detector.h"
#include "material.h"
#include "geometry.h"



BaseDetectorElement::BaseDetectorElement(fastsim::enumLayer layer,
      const  VolumeCylinder& volume , const Material& material) :
   m_volume(volume), m_material(material), m_layer(layer)
{}

BaseDetector::BaseDetector()
{

}


//aim to make this a const function and to do sort at initialisation
const std::list<SurfaceCylinder>& BaseDetector::getSortedCylinders()
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

//const DetectorElement& BaseDetector::getElement(fastsim::enumLayer layer) const
std::shared_ptr<const DetectorElement> BaseDetector::getElement(
   fastsim::enumLayer layer) const
{

   switch (layer) {
      case fastsim::enumLayer::ECAL:
         return m_ECAL;
         break;
      case fastsim::enumLayer::HCAL:
         return m_HCAL;
         break;
      case fastsim::enumLayer::NONE:
      case fastsim::enumLayer::TRACKER:
      case fastsim::enumLayer::__COUNT: //hmmm yucky side effect of clever enums
         return m_ECAL; //TODO
         break;
         //TODO add track and field
   }


   //TODO is this the best route or should it throw
   return nullptr;
}

/*
 Experiments

 BaseDetector::BaseDetector(const DetectorElement & ecal) : m_ECAL(&ecal),  m_cylinders()
 {
 }
 BaseDetector::BaseDetector(std::shared_ptr<const DetectorElement> ecal,std::shared_ptr<const DetectorElement> hcal) : m_ECAL(ecal),m_HCAL(hcal)
 {
 }*/
/*BaseDetector::BaseDetector(DetectorElement && ecal,DetectorElement && hcal) : m_ECAL(std::move(ecal)),m_HCAL(std::move(hcal))
 {
 }

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
