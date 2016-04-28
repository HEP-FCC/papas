//
//  Created by Alice Robson on 09/11/15.
//
//
#include <vector>
#include <list>
#include "detector.h"
#include "material.h"
#include "geometry.h"


DetectorElement::DetectorElement(fastsim::enumLayer layer, const  VolumeCylinder& volume , const Material& material) :
   m_volume(volume), m_material(material), m_layer(layer)
{}

Detector::Detector()
{

}


//aim to make this a const function and to do sort at initialisation
const std::list<SurfaceCylinder>& Detector::sortedCylinders()
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

std::shared_ptr<const Calorimeter> Detector::calorimeter(fastsim::enumLayer layer) const{
  switch (layer) {
    case fastsim::enumLayer::ECAL:
      return m_ecal;
      break;
    case fastsim::enumLayer::HCAL:
      return m_hcal;
      break;
    default:
      //throw error
      return m_ecal;
  }
}

//const DetectorElement& Detector::element(fastsim::enumLayer layer) const
std::shared_ptr<const DetectorElement> Detector::element(
   fastsim::enumLayer layer) const
{

   switch (layer) {
      case fastsim::enumLayer::ECAL:
         return m_ecal;
         break;
      case fastsim::enumLayer::HCAL:
         return m_hcal;
         break;
     default:
         return m_ecal; //TODO throw error
         break;
         //TODO add track and field
   }


   //TODO is this the best route or should it throw
   return nullptr;
}

Field::Field(fastsim::enumLayer layer, const  VolumeCylinder& volume , const Material& material, double magnitude) :
DetectorElement(layer,volume,material), m_magnitude(magnitude)
{}

/*
 Experiments

 Detector::Detector(const DetectorElement & ecal) : m_ECAL(&ecal),  m_cylinders()
 {
 }
 Detector::Detector(std::shared_ptr<const DetectorElement> ecal,std::shared_ptr<const DetectorElement> hcal) : m_ECAL(ecal),m_Hcal(hcal)
 {
 }*/
/*Detector::Detector(DetectorElement && ecal,DetectorElement && hcal) : m_ECAL(std::move(ecal)),m_Hcal(std::move(hcal))
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
