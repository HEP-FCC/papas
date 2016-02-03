//
//  distance.h
//  fastsim
//
//  Created by Alice Robson on 01/02/16.
//
//

#ifndef distance_h
#define distance_h

#include <stdio.h>
#include "enums.h"

struct DistanceData {   fastsim::enumLayer m_layer1;
   fastsim::enumLayer m_layer2;
   bool m_isOK;
   double m_distance;
};

#endif /* distance_h */
