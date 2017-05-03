//
//  AliceDisplay.cpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//
#include "papas/display/PFApp.h"
#include "papas/display/GDetector.h"
#include "papas/display/ViewPane.h"

#include <stdio.h>
// ROOT App to allow graphs to be plotted

namespace papas {
PFApp::PFApp() {}

void PFApp::display(const Event& event, const Detector& det) {

  m_display = new PFEventDisplay({ViewPane::Projection::xy, ViewPane::Projection::yz});
  m_gdetector = std::make_shared<GDetector>(det);
  m_display->addToRegister(m_gdetector, 0);
  m_display->drawEvent(event);
}

void PFApp::jpg() { m_display->saveAs(); }

}  // namespace papas
