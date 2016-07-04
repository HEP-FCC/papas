//
//  PFEventDisplay.h
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#ifndef PFEventDisplay_h
#define PFEventDisplay_h

#include "DefinitionsCollections.h"
#include "displaycore.h"

namespace papas {

class PFEvent;
class Simulator;

class PFEventDisplay : public Display {
public:
  PFEventDisplay(std::list<ViewPane::Projection> views = {});
  void drawEvent(const Simulator& simulator, const PFEvent& pfevent);
  void drawComparison(const PFEvent& pfEvent, const Particles& particles);

private:
};

}  // end namespace PFEvent

#endif /* PFEventDisplay_h */
