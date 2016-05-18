//
//  PFEventDisplay.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

//#include <stdio.h>

#include "PFEventDisplay.h"
#include "PFEvent.h"
#include "displaypfobjects.h"
#include "Track.h"
#include "Cluster.h"


namespace papas {

PFEventDisplay::PFEventDisplay(std::list<ViewPane::Projection> views)
:Display(views){
}

void PFEventDisplay::drawPFEvent(const PFEvent& pfEvent) {
  for (auto& cl : pfEvent.ecalClusters()) {
    std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster,2);
  }
  for (auto& cl : pfEvent.hcalClusters()) {
    std::cout << cl.second;
    std::shared_ptr<GTrajectories> gcluster(new GTrajectories(cl.second));
    addToRegister(gcluster,2);
  }
  for (auto& tr : pfEvent.tracks()) {
    std::shared_ptr<GTrajectories> gtrack(new GTrajectories(tr.second));
    addToRegister(gtrack,2);
  }
  draw();
}
} // end namespace papas