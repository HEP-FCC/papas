//
//  PFEventDisplay.h
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#ifndef PFEventDisplay_h
#define PFEventDisplay_h

#include "displaycore.h"


namespace papas {
  
class PFEvent;

class PFEventDisplay: public Display {
public:
  PFEventDisplay(std::list<ViewPane::Projection> views = {});
  void drawPFEvent(const PFEvent& pfevent);
private:
};
  
} // end namespace PFEvent
  
 
#endif /* PFEventDisplay_h */
