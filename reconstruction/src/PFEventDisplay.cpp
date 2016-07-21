//
//  PFEventDisplay.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "PFEvent.h"
#include "Drawable.h"
#include "GTrajectories.h"
#include "PFEventDisplay.h"
#include "Simulator.h"
#include "ViewPane.h"
#include <iostream>

namespace papas {

PFEventDisplay::PFEventDisplay(std::list<ViewPane::Projection> views) : Display(views) {}

void PFEventDisplay::drawEvent(const Simulator& simulator, const PFEvent& pfEvent) {
  for (auto& pr : simulator.particles()) {
    std::shared_ptr<GTrajectories> gparticle(new GTrajectories(pr.second));
    addToRegister(gparticle, 2);
  }
  for (auto& cl : simulator.smearedEcalClusters()) {
    // std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster, 2);
  }
  for (auto& cl : simulator.smearedHcalClusters()) {
    // std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster, 2);
  }

  draw();
}

void PFEventDisplay::drawComparison(const PFEvent& pfEvent, const Particles& particles) {

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
  draw();
}

}  // end namespace papas