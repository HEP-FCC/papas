//
//  Created by Alice Robson on 09/11/15.
//
//
#include "displaygeometry.h"
#include <memory>


std::map<std::string, int> goldCOLORMap {
   {"ECAL" , kRed - 10},
   {"HCAL" , kBlue - 10}
};

std::map<fastsim::enumLayer, int> gCOLORMap {
   {fastsim::enumLayer::ECAL , kRed - 10},
   {fastsim::enumLayer::HCAL , kBlue - 10}
};

void GDetectorElement::Draw(const std::string& projection)
const //AJRTODO change to an enum?
{
   if (projection == "xy") {

      for (auto const& circle : m_circles) {
         circle->Draw("same");
      }
   } else if (projection == "yz" |  projection == "xz") {

      for (auto const& box : m_boxes) {
         box->Draw("samel");
      }
   } else if (projection.find("thetaphi") != projection.npos) {
   }
   //else
   //AJRTODO throw error
};


Drawable::Drawable()
{
   ;
}

GDetector::GDetector(const BaseDetector& detector) :
   m_gECAL(detector.getECAL()), m_gHCAL(detector.getHCAL())
{

}

void GDetector::Draw(const std::string& projection) const
{

   m_gHCAL.Draw(projection);
   m_gECAL.Draw(projection);
}

GDetectorElement::GDetectorElement(std::shared_ptr<const DetectorElement> de) :
//GDetectorElement::GDetectorElement(const DetectorElement& de) :
   m_detElem(de)
{
   //std::list<SurfaceCylinder* > cylinders{m_detElem->getVolume()->Outer(),
   //   m_detElem->getVolume()->Inner()};

   //For each inner and outer cyclinder create a cyclinder (circular cross section) for xy , yz, xz projections
   //        create a box (longways cross section) for thetaphi projections

   for (auto const& elem : {m_detElem->getVol().Outer(), m_detElem->getVol().Inner()}) { //AJRTODO should be if inner ......
      double radius =  elem.getRadius();
      double dz = elem.Z();

      m_circles.push_back(std::unique_ptr<TEllipse> { new TEllipse(0., 0., radius, radius)});
      m_boxes.push_back(std::unique_ptr<TBox> { new TBox(-dz, -radius, dz, radius)});
   }

   //Choose color according to which element it is
   int color = gCOLORMap[m_detElem->getLayer()];
   for (auto& shape : m_circles) {
      shape->SetFillColor(color);
      shape->SetFillStyle(1001);
      // else: //TODOAJR
      //     shape.SetFillStyle(0);
      shape->SetLineColor(1);
      shape->SetLineStyle(1) ;
      color = 0; //Set color = 0 for inner circle
   }
}
/*

 void GDetectorElement::Draw(std::string projection){
 if  (projection == "xy"){
 for (auto circle : m_circles) {
 circle.Draw("same");
 else if (projection=="xz"|projection=="yz") {
 for (auto box  :boxes) {
 box.Draw('samel')
 else if (view.find("thetaphi")<view.npos)
 {}*/
//AJRTOD throw error else:
//raise ValueError('implement drawing for projection ' + projection )
//}

/*if self.desc.volume.inner:
 self.circles.append( TEllipse(0., 0.,
 self.desc.volume.inner.rad,
 self.desc.volume.inner.rad))
 dz = self.desc.volume.inner.z
 radius = self.desc.volume.inner.rad
 self.boxes.append( TBox(-dz, -radius, dz, radius) ) */

/*for shape in [oc, ob]:
 if color:
 shape.SetFillColor(color)
 shape.SetFillStyle(1001)
 else:
 shape.SetFillStyle(0)
 shape.SetLineColor(1)
 shape.SetLineStyle(1)
 if len(self.circles)==2:
 ic = self.circles[1]
 ib = self.boxes[1]
 for shape in [ic, ib]:
 if color:
 shape.SetFillColor(0)
 shape.SetFillStyle(1001)
 else:
 shape.SetFillStyle(0)*/


/*
 if __name__ == '__main__':

 from ROOT import TCanvas, TH2F
 from heppy_fcc.fastsim.detectors.CMS import CMS
 from heppy_fcc.display.core import Display

 cms = CMS()
 gcms = GDetector(cms)

 display = Display()
 display.register(gcms, 0)
 display.draw()
 */