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

struct DistanceData {   //fastsim::enumLayer m_layer1;
                        //fastsim::enumLayer m_layer2;
   bool isOK;
   double distance;
};


#include "deltar.h"
class Track;
class Cluster;
class TVector3;

class Distance {
public:
  Distance() ;
  Distance(bool islinked, double dist) ;
  Distance(const Cluster& cluster, const Track& track) ;
  Distance(const Cluster& cluster1, const Cluster& cluster2);
  void setDistanceToPoint(const TVector3& point, const Cluster& cluster);
  bool isLinked() const {return m_isLinked;};
  double distance() const {return m_distance;};
private:
  double m_distance;
  bool m_isLinked;
};

#endif /* distance_h */
