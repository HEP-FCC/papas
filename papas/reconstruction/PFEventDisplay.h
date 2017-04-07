#ifndef PFEventDisplay_h
#define PFEventDisplay_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Event.h"
#include "papas/display/Display.h"
#include "papas/display/ViewPane.h"

namespace papas {

/// prototype only
class PFEventDisplay : public Display {
public:
  PFEventDisplay(std::list<ViewPane::Projection> views = {});
  void drawEvent(const Event& event);
  // void drawComparison(const PFEvent& pfEvent, const Particles& particles);

private:
};

}  // end namespace PFEvent

#endif /* PFEventDisplay_h */
