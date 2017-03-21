//
//  PFEventDisplay.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "papas/reconstruction/PFEventDisplay.h"
#include "papas/datatypes/Event.h"
#include "papas/display/Drawable.h"
#include "papas/display/GTrajectories.h"
#include "papas/display/ViewPane.h"
#include <iostream>

namespace papas {

PFEventDisplay::PFEventDisplay(std::list<ViewPane::Projection> views) : Display(views) {}

void PFEventDisplay::drawEvent(const Event& event) {
  for (const auto& pr : event.particles('s')) {
    std::shared_ptr<GTrajectories> gparticle(new GTrajectories(pr.second));
    addToRegister(gparticle, 2);
  }
  for (const auto& cl : event.clusters("es")) {
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster, 2);
  }
  for (const auto& cl : event.clusters("hs")) {
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster, 2);
  }

  draw();
}


}  // end namespace papas
