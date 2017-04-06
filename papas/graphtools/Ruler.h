#ifndef Ruler_h
#define Ruler_h

namespace papas {

// forward declaration
class Distance;
class Cluster;
class Track;

/// Used to calculate distances between particle flow elements (clusters and track)
class Ruler {
public:
  Ruler(){};
  /**
   *   @brief  distance between two clusters
   *
   *   @param[in]  cluster1 First cluster (order does not matter)
   *   @param[in]  cluster2 Second cluster (order does not matter)
   *   @return  Distance (ie isLinked : boolean T/F and distance value)
   */
  Distance clusterClusterDistance(const Cluster& cluster1, const Cluster& cluster2) const;
  /**
   *   @brief  distance between a cluster and a trak
   *
   *   @param[in]  cluster
   *   @param[in]  track
   *   @return  Distance (ie isLinked : boolean T/F and distance value)
   */
  Distance clusterTrackDistance(const Cluster& cluster, const Track& track) const;
};
}  // end namespace papas

#endif /* Ruler_hpp */
