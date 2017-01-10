//
//  PFEventDisplay.h
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#ifndef PFEventDisplay_h
#define PFEventDisplay_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/display/Display.h"
#include "papas/display/ViewPane.h"
#include "papas/datatypes/PapasEvent.h"

namespace papas {

class PFEvent;
class Simulator;

/// prototype only
class PFEventDisplay : public Display {
public:
  PFEventDisplay(std::list<ViewPane::Projection> views = {});
  void drawEvent(const PapasEvent& papasEvent);
  //void drawComparison(const PFEvent& pfEvent, const Particles& particles);

private:
};

}  // end namespace PFEvent

#endif /* PFEventDisplay_h */
