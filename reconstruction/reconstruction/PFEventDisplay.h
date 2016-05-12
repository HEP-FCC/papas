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

class PFEvent;

class PFEventDisplay: public Display {
public:
  PFEventDisplay(std::list<Projection> views = {});
  void drawPFEvent(const PFEvent& pfevent);
private:
};


#endif /* PFEventDisplay_h */
