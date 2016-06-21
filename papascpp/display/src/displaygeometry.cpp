//
//  Created by Alice Robson on 09/11/15.
//
//
#include "displaygeometry.h"
#include <memory>

namespace papas {


std::map<std::string, int> goldCOLORMap {
   {"Ecal" , kRed - 10},
   {"Hcal" , kBlue - 10}
};

std::map<papas::Layer, int> gCOLORMap {
   {papas::Layer::kEcal , kRed - 10},
   {papas::Layer::kHcal , kBlue - 10}
};
  
/*GDetectorElement::~GDetectorElement() {
    //TODO
}*/

void GDetectorElement::Draw(const std::string& projection) const 
{
   if (projection == "xy") {
      for (auto const& circle : m_circles)
         circle->Draw("same");
   }
   else if ( (projection == "yz") |  (projection == "xz")) {
      for (auto const& box : m_boxes) {
         box->Draw("samel");
      }
   }
   else if (projection.find("thetaphi") != projection.npos) {
   }
   //else
   //AJRTODO throw error
};

Drawable::Drawable()
{
}
  
/*Drawable::~Drawable()
{
}*/

GDetector::GDetector(const Detector& detector):
   m_gEcal(detector.ecal()),
   m_gHcal(detector.hcal())
{
}
  
/*GDetector::~GDetector() {
  //TODO
}*/

void GDetector::Draw(const std::string& projection) const {
   m_gHcal.Draw(projection);
   m_gEcal.Draw(projection);
}

GDetectorElement::GDetectorElement(std::shared_ptr<const DetectorElement> de) :
//GDetectorElement::GDetectorElement(const DetectorElement& de) :
   m_detElem(de)
{
   //std::list<SurfaceCylinder* > cylinders{m_detElem->volume()->Outer(),
   //   m_detElem->volume()->Inner()};

   //For each inner and outer cyclinder create a cyclinder (circular cross section) for xy , yz, xz projections
   //        create a box (longways cross section) for thetaphi projections

   for (auto const& elem : {m_detElem->volumeCylinder().outer(), m_detElem->volumeCylinder().inner()}) { //AJRTODO should be if inner ......
      double radius =  elem.getRadius();
      double dz = elem.Z();

      m_circles.push_back(std::unique_ptr<TEllipse> { new TEllipse(0., 0., radius, radius)});
      m_boxes.push_back(std::unique_ptr<TBox> { new TBox(-dz, -radius, dz, radius)});
   }

   //Choose color according to which element it is
   int color = gCOLORMap[m_detElem->layer()];
   for (auto& shape : m_circles) {
      shape->SetFillColor(color);
      shape->SetFillStyle(1001);
      // else: //TODOAJR
      //     shape.SetFillStyle(0);
      shape->SetLineColor(1);
      shape->SetLineStyle(1) ;
      color = 0; //Set color = 0 for inner circle
   }
   color = gCOLORMap[m_detElem->layer()];

   for (auto& shape : m_boxes) {
      shape->SetFillColor(color);
      shape->SetFillStyle(1001);
      // else: //TODOAJR
      //     shape.SetFillStyle(0);
      shape->SetLineColor(1);
      shape->SetLineStyle(1) ;
      color = 0; //Set color = 0 for inner
   }
}
  
} // end namespace papas
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
 display.addToRegister(gcms, 0)
 display.draw()
 */
