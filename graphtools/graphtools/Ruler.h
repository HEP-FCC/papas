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
class Cluster;
class Track;
class Distance;
class PFEvent;

//TODO
typedef long longId;

//TODO template on Distance class
class Ruler {
  
public:
  Ruler (const PFEvent& pfevent);
  Distance distance(longId id1 ,longId id2);
private:
  Distance clusterClusterDistance(longId id1 ,longId id2);
  Distance clusterTrackDistance(longId id1 ,longId id2);
  Distance distance();
  
  const PFEvent& m_pfEvent;
};


  





#endif /* Ruler_hpp */
