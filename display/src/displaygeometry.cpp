//
//  Created by Alice Robson on 09/11/15.
//
//
#include "displaygeometry.h"
#include <memory>

namespace papas {

std::map<std::string, int> goldCOLORMap{{"Ecal", kRed - 10}, {"Hcal", kBlue - 10}};

std::map<papas::Layer, int> gCOLORMap{{papas::Layer::kEcal, kRed - 10}, {papas::Layer::kHcal, kBlue - 10}};

/*GDetectorElement::~GDetectorElement() {
    //TODO
}*/

void GDetectorElement::Draw(const std::string& projection)  {
  if (projection == "xy") {
    for (auto & circle : m_circles)
      circle.Draw("same");
  } else if ((projection == "yz") | (projection == "xz")) {
    for (auto & box : m_boxes) {
      box.Draw("samel");
    }
  } else if (projection.find("thetaphi") != projection.npos) {
  }
  // else
  // AJRTODO throw error
};

Drawable::Drawable() {}


GDetector::GDetector(const Detector& detector) : m_gEcal(detector.ecal()), m_gHcal(Detector.hcal()) {}


void GDetector::Draw(const std::string& projection) {
  m_gHcal.Draw(projection);
  m_gEcal.Draw(projection);
}

void GDetector::DrawSimple(const std::string& projection) const {
  auto gelem = GDetectorElement(3., 2);
  gelem.Draw(projection);
}

GDetectorElement::GDetectorElement(std::shared_ptr<const DetectorElement> detElem) {  // For each inner and outer
                                                                                      // cyclinder create a cyclinder
                                                                                      // (circular cross section) for xy
                                                                                      // , yz, xz projections
  //        create a box (longways cross section) for thetaphi projections

  for (auto const& elem :
       {detElem->volumeCylinder().outer(), detElem->volumeCylinder().inner()}) {  // AJRTODO should be if inner ......
    double radius = elem.getRadius();
    double dz = elem.Z();

    m_circles.push_back(std::move(TEllipse(0., 0., radius, radius)));
    m_boxes.push_back(std::move(TBox(-dz, -radius, dz, radius)));
  }

  // Choose color according to which element it is
  int color = gCOLORMap[detElem->layer()];
  for (auto& shape : m_circles) {
    shape.SetFillColor(color);
    shape.SetFillStyle(1001);
    // else: //TODOAJR
    //     shape.SetFillStyle(0);
    shape.SetLineColor(1);
    shape.SetLineStyle(1);
    color = 0;  // Set color = 0 for inner circle
  }
  color = gCOLORMap[detElem->layer()];

  for (auto& shape : m_boxes) {
    shape.SetFillColor(color);
    shape.SetFillStyle(1001);
    // else: //TODOAJR
    //     shape.SetFillStyle(0);
    shape.SetLineColor(1);
    shape.SetLineStyle(1);
    color = 0;  // Set color = 0 for inner
  }
}

GDetectorElement::GDetectorElement(double radius, double dz) {

  m_circles.push_back(std::move(TEllipse(0., 0., radius, radius)));
  m_boxes.push_back(std::move(TBox(-dz, -radius, dz, radius)));

  // Choose color according to which element it is
  int color = 0;
  for (auto& shape : m_circles) {
    shape.SetFillColor(color);
    shape.SetFillStyle(1001);
    shape.SetLineColor(1);
    shape.SetLineStyle(1);
    color = 0;  // Set color = 0 for inner circle
  }
  for (auto& shape : m_boxes) {
    shape.SetFillColor(color);
    shape.SetFillStyle(1001);
    // else: //TODOAJR
    //     shape.SetFillStyle(0);
    shape.SetLineColor(1);
    shape.SetLineStyle(1);
    color = 0;  // Set color = 0 for inner
  }
}

}  // end namespace papas
