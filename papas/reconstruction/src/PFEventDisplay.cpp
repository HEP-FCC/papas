//
//  PFEventDisplay.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "papas/datatypes/PapasEvent.h"
#include "papas/display/Drawable.h"
#include "papas/display/GTrajectories.h"
#include "papas/reconstruction/PFEventDisplay.h"
#include "papas/display/ViewPane.h"
#include <iostream>

namespace papas {

PFEventDisplay::PFEventDisplay(std::list<ViewPane::Projection> views) : Display(views) {}

void PFEventDisplay::drawEvent(const PapasEvent& papasEvent) {
  for (auto& pr : papasEvent.particles('s')) {
    std::shared_ptr<GTrajectories> gparticle(new GTrajectories(pr.second));
    addToRegister(gparticle, 2);
  }
  for (auto& cl : papasEvent.clusters("es")) {
    // std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster, 2);
  }
  for (auto& cl : papasEvent.clusters("hs")) {
    // std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster, 2);
  }

  draw();
}

  //void PFEventDisplay::drawComparison(const PFEvent& pfEvent, const Particles& particles) {

  /*for (auto& pr : pfEvent.reconstructedParticles()) {
    std::shared_ptr<GTrajectories> gparticle(new GTrajectories(pr.second, 1, 3, 1));
    addToRegister(gparticle, 2);
  }
  for (auto& pr : particles) {
    std::shared_ptr<GTrajectories> gparticle(new GTrajectories(pr.second, 1, 1, 1));
    addToRegister(gparticle, 2);
  }
  for (auto& pr : pfEvent.reconstructedParticles()) {
    std::shared_ptr<GTrajectories> gparticle(new GTrajectories(pr.second, 2, 2, 2));
    addToRegister(gparticle, 2);
  }*/
  //draw();
  //}

}  // end namespace papas
