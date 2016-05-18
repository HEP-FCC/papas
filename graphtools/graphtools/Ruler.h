//
//  Ruler.hpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#ifndef Ruler_h
#define Ruler_h

#include <stdio.h>
#include "Id.h"

namespace papas {
  
class Cluster;
class Track;
class Distance;
class PFEvent;



//TODO template on Distance class
class Ruler {
  
public:
  Ruler (const PFEvent& pfevent);
  Distance distance(Id::Type id1 ,Id::Type id2);
private:
  Distance clusterClusterDistance(Id::Type id1 ,Id::Type id2);
  Distance clusterTrackDistance(Id::Type id1 ,Id::Type id2);
  Distance distance();
  
  const PFEvent& m_pfEvent;
};
} // end namespace papas

#endif /* Ruler_hpp */
