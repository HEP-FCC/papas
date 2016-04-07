#ifndef RECONSTRUCTION_PFEVENT_H
#define RECONSTRUCTION_PFEVENT_H

#include <iostream>
#include <vector>
#include <unordered_map>

/** @class   rec::PFEvent Reconstruction/Reconstruction/PFEvent.h PFEvent.h
 *
 *  @brief An PFEvent created blocks of connected elements
 *
 *  Example usage: PFEvent b = PFEvent...;
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */
#include "identifier.h"


class PFEvent {
public:
  typedef long longID;
  PFEvent();
  bool compare(longID id1, longID id2) const;
  double getEnergy(longID id1) const;

private:
  
};

#endif /* PFEvent_h */
