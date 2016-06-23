//
//  AliceDisplay.cpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//
#include "AliceDisplay.h"
#include <stdio.h>
// ROOT App to allow graphs to be plotted

namespace papas {
PFApp::PFApp() :
m_argc(0), m_theApp("App", &m_argc ,m_str) {
}

void PFApp::display(const PFEvent& pfEvent, const Detector& det) {
  
  m_display = new PFEventDisplay({ViewPane::Projection::xy, ViewPane::Projection::yz});
  m_gdetector =  std::make_shared<GDetector>(GDetector(det));
  m_display->addToRegister(m_gdetector, 0);
  m_display->drawPFEvent(pfEvent);
}
}