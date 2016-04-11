//
//  PFEvent.cpp
//  fastsim
//
//  Created by Alice Robson on 06/04/16.
//
//

#include "PFEvent.h"
#include "PFBlock.h"
PFEvent::PFEvent()
{
  
}

bool PFEvent::compare(longID id1, longID id2) const //TODO check direction of sort
{
  //sort by the type eg ecal hcal
  // and then in order of decreasing energy
  char c1 = Identifier::typeShortCode(id1);
  char c2 = Identifier::typeShortCode(id2);
  if ( c1 < c2)
    return true;
  else if (c2 > c1)
    return false;
  else if (getEnergy(id1) > getEnergy(id2)) // c1 == c2
    return true;
  return false;
  
}


double PFEvent::getEnergy(longID id1) const //TODO check direction of sort
{
  return 12.5; //TODO
  
}

