//
//  Ruler.hpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#ifndef Ruler_h
#define Ruler_h

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Track.h"

namespace papas {

class Distance;

/// Used to calculate distances between particle flow elements (clusters and track)
class Ruler {

public:
  Ruler(){};
  Distance clusterClusterDistance(const Cluster& cluster1, const Cluster& cluster2) const;
  Distance clusterTrackDistance(const Cluster& cluster, const Track& track) const;
};
}  // end namespace papas

#endif /* Ruler_hpp */
