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
class Cluster;
class Track;
class Distance;
class PFEvent;



//TODO template on Distance class
class Ruler {
  
public:
  Ruler (const PFEvent& pfevent);
  Distance distance(Id::type id1 ,Id::type id2);
private:
  Distance clusterClusterDistance(Id::type id1 ,Id::type id2);
  Distance clusterTrackDistance(Id::type id1 ,Id::type id2);
  Distance distance();
  
  const PFEvent& m_pfEvent;
};


  





#endif /* Ruler_hpp */
